# 📊 Diagrammes et Exemples Visuels

## Flux de Données CUDA

### 1. Pipeline Complet - GetClosestScanToPose

```
┌─────────────────────────────────────────────────────────────────┐
│  SLAM Mapper: Besoin de trouver scan le plus proche             │
└────────────────────┬────────────────────────────────────────────┘
                     │
                     ▼
        ┌────────────────────────────┐
        │ GetClosestScanToPose()     │
        │ (5000 scans, 1 pose_ref)   │
        └────────────┬───────────────┘
                     │
                     ▼
        ┌────────────────────────────────────────┐
        │ ComputeDistancesGPU()                  │
        │ - Convertir Karto types → floats       │
        │ - Appeler kernel CUDA                  │
        │ - Retourner distances                  │
        └────────────┬───────────────────────────┘
                     │
         ┌───────────┴───────────┐
         │                       │
         ▼                       ▼
    ┌─────────────┐      ┌──────────────────────┐
    │ MALLOC GPU  │      │ MEMCPY CPU → GPU     │
    │             │      │ - poses_x            │
    │ d_poses_x   │      │ - poses_y            │
    │ d_poses_y   │      │ (1 ms)               │
    │ d_distances │      └──────────────────────┘
    │ (0.1 ms)    │
    └─────────────┘
         │
         └────────────────┬─────────────────────┐
                          │                     │
                          ▼                     ▼
                   ┌──────────────────────────────────┐
                   │   KERNEL LAUNCH                  │
                   │   computeDistancesKernel         │
                   │   <<<4 blocks, 256 threads>>>    │
                   │                                  │
                   │   Block 0: threads 0-255         │
                   │   Block 1: threads 256-511       │
                   │   Block 2: threads 512-767       │
                   │   Block 3: threads 768-999       │
                   │                                  │
                   │   Chaque thread:                 │
                   │   idx = thread_idx               │
                   │   dx = x[idx] - x_ref            │
                   │   dy = y[idx] - y_ref            │
                   │   dist[idx] = dx² + dy²          │
                   │                                  │
                   │   ⏱️ 0.1 ms (parallèle!)         │
                   └──────────────────────────────────┘
         │
         └────────────────┬──────────────────────┬─────────┐
                          │                      │         │
                          ▼                      ▼         ▼
                 ┌─────────────────┐  ┌──────────────────────┐
                 │ SYNCHRONIZE GPU │  │ MEMCPY GPU → CPU     │
                 │ (0.05 ms)       │  │ distances array      │
                 └─────────────────┘  │ (1 ms)               │
                 │                    └──────────────────────┘
                 │                      │
                 └──────────┬───────────┘
                            │
                            ▼
                   ┌──────────────────┐
                   │ FREE GPU MEMORY  │
                   │ (0.01 ms)        │
                   └──────────────────┘
                            │
                            ▼
        ┌────────────────────────────────┐
        │ Find Min on CPU (rapide!)      │
        │ for i in distances:            │
        │   if distances[i] < best:      │
        │     best_idx = i               │
        │ ⏱️ 0.1 ms                      │
        └────────────┬───────────────────┘
                     │
                     ▼
        ┌────────────────────────────┐
        │ RETURN closest scan        │
        │ (rScans[best_idx])         │
        └────────────────────────────┘

TOTAL: ~2-3 ms pour 5000 scans (vs 5-10 ms CPU seul)
```

---

## 2. Architecture GPU - Grille de Threads

```
Memory Layout pour 5000 scans:

┌─────────────────────────────────────────────────────┐
│                    GPU GRILLE                       │
├─────────────────────────────────────────────────────┤
│                                                     │
│  BLOCK 0          BLOCK 1        BLOCK 2   BLOCK 3 │
│  ┌──────────────┐ ┌──────────────┐ ...     ...    │
│  │ 256 threads  │ │ 256 threads  │                 │
│  │              │ │              │                 │
│  │ Thread 0-255 │ │ Thread 256.. │                 │
│  │              │ │              │                 │
│  │ idx = 0-255  │ │ idx = 256... │                 │
│  └──────────────┘ └──────────────┘                 │
│                                                     │
│  4 blocs × 256 threads = 1024 threads             │
│  Mais kernel appelé avec numPoses=5000            │
│  Donc threads 1024-4999 doivent être lancés      │
│  (Peut être fait avec plus de grilles si ++)     │
│                                                     │
└─────────────────────────────────────────────────────┘

Calcul des indices globaux:
┌────────────────────────────────────┐
│ Exemple: Thread dans Block 2       │
│                                    │
│ blockIdx.x = 2                     │
│ blockDim.x = 256                   │
│ threadIdx.x = 100                  │
│                                    │
│ idx = 2 * 256 + 100 = 612          │
│                                    │
│ Traite scan #612                   │
└────────────────────────────────────┘
```

---

## 3. Mémoire GPU - Allocation et Transfert

```
CPU SIDE (Host Memory)
┌──────────────────────────────────┐
│ poses_x: [1.5, 2.3, ... 5000]   │ ← H2D Transfer
│ poses_y: [2.1, 3.5, ... 5000]   │
│ distances: [?, ?, ... ?]        │ ← D2H Transfer
└───────────┬──────────────────────┘
            │
            │  cudaMemcpy(..., cudaMemcpyHostToDevice)
            ▼
GPU SIDE (Device Memory)
┌──────────────────────────────────┐
│ d_poses_x: [1.5, 2.3, ... 5000]  │
│ d_poses_y: [2.1, 3.5, ... 5000]  │
│ d_distances: [uninitialized]     │
└───────────┬──────────────────────┘
            │
            │  computeDistancesKernel<<<4, 256>>>()
            │  Chaque thread fait:
            │    d_distances[idx] = (x[idx]-ref_x)² + ...
            ▼
┌──────────────────────────────────┐
│ d_distances: [5.2, 7.1, ... 3.2] │ ← Calculated
└───────────┬──────────────────────┘
            │
            │  cudaMemcpy(..., cudaMemcpyDeviceToHost)
            ▼
CPU SIDE
┌──────────────────────────────────┐
│ distances: [5.2, 7.1, ... 3.2]   │ ← Ready to use
└──────────────────────────────────┘
```

---

## 4. Comparaison Temporal - CPU vs GPU

```
CPU APPROCHE (Séquentielle):

Timeline:
0 ms    ├─ Scan 1: calc dist ────────────
1 ms    ├─ Scan 2: calc dist ────────────
2 ms    ├─ Scan 3: calc dist ────────────
3 ms    ├─ Scan 4: calc dist ────────────
...     ...
5000 ms ├─ Scan 5000: calc dist ────────
5000 ms ├─ Find min: 0.1 ms ─
5000.1 ms = Total

GPU APPROCHE (Parallèle):

Timeline:
0 ms    ├─ Malloc GPU: 0.1 ms ─
0.1 ms  ├─ Memcpy H→D: 1 ms ──────────
1.1 ms  ├─ Kernel (tous en //) ─ 0.1 ms
1.2 ms  ├─ Sync: 0.05 ms ─
1.25 ms ├─ Memcpy D→H: 1 ms ──────────
2.25 ms ├─ Free: 0.01 ms
2.26 ms ├─ Find min: 0.1 ms ─
2.36 ms = Total

SPEEDUP: 5000 / 2.36 ≈ 2116× dans cet exemple!
(Mais overhead domine pour petit N)
```

---

## 5. Exemple d'Exécution - Pas à Pas

### Données d'Entrée
```
Pose de référence:    (0.0, 0.0)
Scans à chercher:
  Scan 0: position (1.0, 2.0)  → distance² = 1² + 2² = 5
  Scan 1: position (3.0, 4.0)  → distance² = 3² + 4² = 25
  Scan 2: position (0.5, 0.5)  → distance² = 0.5² + 0.5² = 0.5
  Scan 3: position (-1.0, 1.0) → distance² = (-1)² + 1² = 2
  
  Attendu: Scan 2 est le plus proche!
```

### Exécution CPU
```cpp
GetClosestScanToPose(rScans, refPose):
    bestDistance = DBL_MAX
    
    i=0: dist = 5.0     → best = 5.0
    i=1: dist = 25.0    → non, garder 5.0
    i=2: dist = 0.5     → best = 0.5   ← NEW MIN!
    i=3: dist = 2.0     → non, garder 0.5
    
    return rScans[2]

⏱️ 4 itérations × 1 μs = 4 μs
```

### Exécution GPU
```cuda
// Kernel lancé avec 1 block, 4 threads

Thread 0: distances[0] = 1² + 2² = 5.0
Thread 1: distances[1] = 3² + 4² = 25.0
Thread 2: distances[2] = 0.5² + 0.5² = 0.5  
Thread 3: distances[3] = (-1)² + 1² = 2.0

// Tous en PARALLÈLE! (simultané)

Sync + copy back:
// CPU trouve le min dans le tableau:
best = 0.5 (index 2)

return rScans[2]

⏱️ Parallèle: ~2 μs (vs 4 μs séquentiel)
  + overhead transfert: ~1 ms
  = ~1 ms total (pour petit exemple, overhead domine!)
```

---

## 6. Architecture du Processus SLAM avec CUDA

```
INPUT: Scan laser du robot

┌──────────────────────────────────┐
│ ProcessScan()                    │
│ (Karto.cpp)                      │
└────────────┬─────────────────────┘
             │
             ▼
    ┌────────────────────┐
    │ AddVertex()        │
    │ Ajouter au graphe  │
    └────────┬───────────┘
             │
             ▼
    ┌─────────────────────────────────┐
    │ AddEdges()                      │
    │ Lier au précédent + running...  │
    └────────┬────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────────┐
    │ LinkChainToScan()                    │
    │ ├─ GetClosestScanToPose() ← GPU ✨  │
    │ ├─ MatchScan()                      │
    │ └─ CreateEdge()                     │
    └────────┬─────────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────────┐
    │ TryCloseLoop()                       │
    │ ├─ FindPossibleLoopClosure()         │
    │ ├─ MatchScan() (coarse)             │
    │ ├─ MatchScan() (fine)  ← Could GPU! │
    │ └─ CorrectPoses()                   │
    └────────┬─────────────────────────────┘
             │
             ▼
        OUTPUT: Map mis à jour

GPU OPTIMIZATIONS:
✨ GetClosestScanToPose - DÉJÀ FAIT
⭐ FindNearByScans - POSSIBLE
⭐ Scan matching - POSSIBLE (advanced)
```

---

## 7. Comparaison Visuelle - Allocation Mémoire

```
SANS CUDA (CPU only):
┌─────────────────┐
│ Stack:          │ 1 MB
│ - Locals        │
└─────────────────┘
┌─────────────────┐
│ Heap:           │ Quelques MB
│ - Scans data    │
│ - Graph nodes   │
└─────────────────┘

AVEC CUDA:
┌─────────────────┐
│ CPU:            │
│ Stack + Heap    │ ← Comme avant
└─────────────────┘
             │
             ▼
┌──────────────────────────┐
│ GPU VRAM:                │ (depend GPU)
│ d_poses_x: 5000 float    │ 20 KB
│ d_poses_y: 5000 float    │ 20 KB  
│ d_distances: 5000 float  │ 20 KB
│ ─────────────────────────│ Total: 60 KB
│ Très petit! (GPU a GBs)  │ (1-4 GB dispo)
└──────────────────────────┘

Mémoire est PAS un problème!
```

---

## 8. Code Simplifié - Pseudo Code

```python
# Pseudo code montrant le concept

def GetClosestScanToPose(scans, reference_pose):
    # ============ GPU PARALLÈLE ============
    distances = ComputeDistancesGPU(scans, reference_pose)
    # distances calculé en parallèle sur GPU
    # =========== FIN PARALLÈLE ==============
    
    # ============ CPU SÉQUENTIEL ============
    best_idx = 0
    for i in range(len(distances)):
        if distances[i] < distances[best_idx]:
            best_idx = i
    # =========== FIN CPU ====================
    
    return scans[best_idx]


def ComputeDistancesGPU(scans, ref_pose):
    # Convertir données Karto → GPU compatible
    x_array = [s.pose.x for s in scans]
    y_array = [s.pose.y for s in scans]
    
    # Allouer mémoire GPU
    d_x = GPU_malloc(len(x_array) * 4)
    d_y = GPU_malloc(len(y_array) * 4)
    d_dist = GPU_malloc(len(scans) * 4)
    
    # Copier vers GPU
    GPU_copy_H2D(d_x, x_array)
    GPU_copy_H2D(d_y, y_array)
    
    # Lancer kernel
    GPU_kernel(d_x, d_y, ref_pose.x, ref_pose.y, d_dist)
    
    # Copier résultats
    distances = GPU_copy_D2H(d_dist)
    
    # Libérer GPU
    GPU_free(d_x, d_y, d_dist)
    
    return distances
```

---

## 9. Matrice de Décision - Utiliser GPU ou CPU?

```
┌──────────────────────────────────┐
│ Nombre de Scans?                 │
└──────────────┬───────────────────┘
               │
        ┌──────┴───────┐
        │              │
     < 100          > 100
        │              │
        ▼              ▼
   CPU OK         CPU/GPU?
        │              │
        │        ┌──────┴──────┐
        │        │             │
    < 500     > 500        GPU Available?
        │        │             │
        │        ▼        ┌────┴────┐
        │      GPU⭐       │         │
        │    5× faster!   YES       NO
        │                  │         │
        │                  ▼         ▼
        │                GPU⭐       CPU OK
        │              Optimal    (fallback)
        │
        └────────┬────────┘
                 │
                 ▼
            DECISION:
         Use ComputeDistancesGPU()
              (auto fallback CPU)
```

---

## 10. Profiling - Où Passe le Temps?

```
Avec CPU seul (5000 scans):
┌──────────────────────────────────────┐
│ GetClosestScanToPose()               │
│ ├─ Loop 5000 scans: 4.9 ms (98%) ██ │
│ └─ Find min: 0.1 ms (2%)          ░  │
│ Total: 5.0 ms                        │
└──────────────────────────────────────┘

Avec GPU (5000 scans):
┌──────────────────────────────────────┐
│ ComputeDistancesGPU()                │
│ ├─ Malloc: 0.1 ms (5%)             ░ │
│ ├─ H2D Transfer: 1.0 ms (50%)   ████ │
│ ├─ Kernel: 0.1 ms (5%)             ░ │
│ ├─ D2H Transfer: 1.0 ms (50%)   ████ │
│ └─ Free: 0.01 ms (<1%)             ░ │
│ GPU: 2.2 ms                         │
│ └─ Find min: 0.1 ms (5%)           ░ │
│ Total: 2.3 ms                       │
└──────────────────────────────────────┘

VERDICT:
- GPU kernel: 0.1 ms (très rapide!)
- Transfers: 2.0 ms (overhead!)
- Seuil rentabilité: N > 500 scans
```

---

**Fin des Diagrammes - Vous avez maintenant une vue complète de l'architecture CUDA! 🎉**
