#include "cuda_mapper_kernels.h"
#include <cuda_runtime.h>
#include <stdio.h>

// ============================================================================
// KERNEL CUDA: Calcul parallèle des distances carrées
// ============================================================================
// Chaque thread calcule la distance² entre une pose de scan et une pose de référence
// Distance² = (x_scan - x_ref)² + (y_scan - y_ref)²
// ============================================================================
__global__ void computeDistancesKernel(
    const float *poses_x, // Positions X des N scans (sur GPU)
    const float *poses_y, // Positions Y des N scans (sur GPU)
    float ref_x,          // Position X de la pose de référence (scalar)
    float ref_y,          // Position Y de la pose de référence (scalar)
    float *distances,     // Tableau résultat des distances² (sur GPU)
    int numPoses          // Nombre total de scans/poses
)
{
    // Calcul de l'index global du thread dans la grille
    // blockIdx.x = numéro du bloc dans X
    // blockDim.x = nombre de threads par bloc en X
    // threadIdx.x = numéro du thread dans le bloc
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Vérifier que le thread ne sort pas des limites du tableau
    if (idx < numPoses)
    {
        // Lire la position du scan
        float dx = poses_x[idx] - ref_x; // Δx
        float dy = poses_y[idx] - ref_y; // Δy

        // Calculer et écrire la distance² en mémoire GPU
        // Cette opération est exécutée en parallèle pour tous les idx
        distances[idx] = dx * dx + dy * dy;
    }
}

// ============================================================================
// FONCTION HOST (CPU): Wrapper pour appeler le kernel CUDA
// ============================================================================
// Gère:
// 1. Allocation mémoire GPU
// 2. Transfert des données CPU → GPU
// 3. Lancement du kernel
// 4. Transfert des résultats GPU → CPU
// 5. Libération mémoire GPU
// ============================================================================
cudaError_t cudaComputeDistances(
    float ref_x,            // Position X de référence
    float ref_y,            // Position Y de référence
    const float *h_poses_x, // Positions X sur CPU (h = host)
    const float *h_poses_y, // Positions Y sur CPU
    int numPoses,           // Nombre de poses
    float *h_distances)     // Résultats (distances²) sur CPU
{
    // *** ÉTAPE 1: Allocation mémoire sur GPU (device) ***
    float *d_poses_x = NULL;   // Pointeur pour positions X (d = device)
    float *d_poses_y = NULL;   // Pointeur pour positions Y
    float *d_distances = NULL; // Pointeur pour distances

    size_t bytes_poses = numPoses * sizeof(float); // Taille en octets

    // Allouer mémoire GPU pour les tableaux
    cudaError_t err = cudaMalloc((void **)&d_poses_x, bytes_poses);
    if (err != cudaSuccess)
        return err;

    err = cudaMalloc((void **)&d_poses_y, bytes_poses);
    if (err != cudaSuccess)
        return err;

    err = cudaMalloc((void **)&d_distances, bytes_poses);
    if (err != cudaSuccess)
        return err;

    // *** ÉTAPE 2: Copie des données CPU → GPU ***
    // cudaMemcpyHostToDevice = copie depuis mémoire CPU vers mémoire GPU
    err = cudaMemcpy(d_poses_x, h_poses_x, bytes_poses, cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        cudaFree(d_poses_x);
        cudaFree(d_poses_y);
        cudaFree(d_distances);
        return err;
    }

    err = cudaMemcpy(d_poses_y, h_poses_y, bytes_poses, cudaMemcpyHostToDevice);
    if (err != cudaSuccess)
    {
        cudaFree(d_poses_x);
        cudaFree(d_poses_y);
        cudaFree(d_distances);
        return err;
    }

    // *** ÉTAPE 3: Configuration et lancement du kernel ***
    // Bloc de 128 threads par bloc = optimal pour Jetson Nano
    // Jetson Nano possède exactement 128 CUDA cores,
    // donc 128 threads occupent tous les cores disponibles
    int blockSize = 128;

    // Nombre de blocs nécessaires pour traiter numPoses
    // Exemple: numPoses=1000, blockSize=128 → numBlocks=8
    int numBlocks = (numPoses + blockSize - 1) / blockSize;

    // Lancer le kernel sur GPU
    // <<<numBlocks, blockSize>>> = config de grille
    // numBlocks blocs × blockSize threads = numPoses threads disponibles
    computeDistancesKernel<<<numBlocks, blockSize>>>(
        d_poses_x, d_poses_y, // Données sur GPU
        ref_x, ref_y,         // Scalaires (pas besoin de copie)
        d_distances,          // Résultats sur GPU
        numPoses);            // Nombre total de poses

    // Vérifier les erreurs de lancement du kernel
    err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        // Libérer mémoire en cas d'erreur
        cudaFree(d_poses_x);
        cudaFree(d_poses_y);
        cudaFree(d_distances);
        return err;
    }

    // Synchroniser: attendre que tous les threads terminent
    // Nécessaire avant de copier les résultats
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess)
    {
        cudaFree(d_poses_x);
        cudaFree(d_poses_y);
        cudaFree(d_distances);
        return err;
    }

    // *** ÉTAPE 4: Copie des résultats GPU → CPU ***
    // cudaMemcpyDeviceToHost = copie depuis mémoire GPU vers CPU
    err = cudaMemcpy(h_distances, d_distances, bytes_poses, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess)
    {
        cudaFree(d_poses_x);
        cudaFree(d_poses_y);
        cudaFree(d_distances);
        return err;
    }

    // *** ÉTAPE 5: Libération de la mémoire GPU ***
    // Important: éviter les fuites mémoire GPU
    cudaFree(d_poses_x);
    cudaFree(d_poses_y);
    cudaFree(d_distances);

    // Retourner le statut de succès
    return cudaSuccess;
}

// ============================================================================
// KERNEL CUDA OPTIMISÉ: Calcul avec réduction min (futur)
// ============================================================================
// Cette version améliorée pourrait aussi retourner l'index du minimum
// directement sur GPU, réduisant les transferts de données
// ============================================================================
/*
__global__ void computeDistancesAndFindMinKernel(
    const float* poses_x,
    const float* poses_y,
    float ref_x,
    float ref_y,
    float* distances,
    int* minIdx,
    float* minDistance,
    int numPoses)
{
    // Shared memory pour réduction parallèle du minimum
    __shared__ float shared_distances[256];
    __shared__ int shared_indices[256];

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Calcul parallèle de la distance
    if (idx < numPoses) {
        float dx = poses_x[idx] - ref_x;
        float dy = poses_y[idx] - ref_y;
        shared_distances[threadIdx.x] = dx * dx + dy * dy;
        shared_indices[threadIdx.x] = idx;
    } else {
        shared_distances[threadIdx.x] = FLT_MAX;
    }

    __syncthreads();

    // Réduction parallèle du minimum dans le bloc
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (threadIdx.x < s) {
            if (shared_distances[threadIdx.x + s] < shared_distances[threadIdx.x]) {
                shared_distances[threadIdx.x] = shared_distances[threadIdx.x + s];
                shared_indices[threadIdx.x] = shared_indices[threadIdx.x + s];
            }
        }
        __syncthreads();
    }

    // Thread 0 écrit le résultat
    if (threadIdx.x == 0) {
        minDistance[blockIdx.x] = shared_distances[0];
        minIdx[blockIdx.x] = shared_indices[0];
    }
}
*/
