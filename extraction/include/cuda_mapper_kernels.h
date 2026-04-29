#ifndef CUDA_MAPPER_KERNELS_H
#define CUDA_MAPPER_KERNELS_H

#include <cuda_runtime.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // ============================================================================
    // FONCTION: Calcul parallèle des distances carrées entre poses
    // ============================================================================
    // Description:
    //   Calcule la distance² entre une pose de référence et N poses de scans
    //   en utilisant le GPU pour le parallélisme.
    //
    // Paramètres:
    //   ref_x          : Position X de la pose de référence (CPU)
    //   ref_y          : Position Y de la pose de référence (CPU)
    //   h_poses_x      : Tableau des positions X des poses (sur CPU)
    //   h_poses_y      : Tableau des positions Y des poses (sur CPU)
    //   numPoses       : Nombre de poses à traiter
    //   h_distances    : Tableau de résultats pour les distances² (sur CPU)
    //
    // Retour:
    //   cudaSuccess si succès, code erreur CUDA sinon
    //
    // Exemple d'utilisation:
    //   std::vector<float> poses_x = {1.0f, 2.0f, 3.0f};
    //   std::vector<float> poses_y = {4.0f, 5.0f, 6.0f};
    //   std::vector<float> distances(3);
    //
    //   cudaComputeDistances(0.0f, 0.0f,
    //                        poses_x.data(), poses_y.data(),
    //                        3, distances.data());
    //
    //   // distances[0] = 1² + 4² = 17
    //   // distances[1] = 2² + 5² = 29
    //   // distances[2] = 3² + 6² = 45
    // ============================================================================
    cudaError_t cudaComputeDistances(
        float ref_x,            // Position X de référence
        float ref_y,            // Position Y de référence
        const float *h_poses_x, // Positions X (sur CPU/host)
        const float *h_poses_y, // Positions Y (sur CPU/host)
        int numPoses,           // Nombre de poses
        float *h_distances);    // Distances résultantes (sur CPU/host)

// ============================================================================
// MACRO DE DIAGNOSTIC: Vérifier les erreurs CUDA
// ============================================================================
#define CHECK_CUDA(ans)                       \
    {                                         \
        gpuAssert((ans), __FILE__, __LINE__); \
    }
    inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
    {
        if (code != cudaSuccess)
        {
            fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
            if (abort)
                exit(code);
        }
    }

#ifdef __cplusplus
}
#endif

#endif // CUDA_MAPPER_KERNELS_H
