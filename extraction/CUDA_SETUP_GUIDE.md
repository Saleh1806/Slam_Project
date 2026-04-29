# Guide de Configuration CUDA pour SLAM Mapper

## Quickstart - Démarrer Rapidement

### 1️⃣ Prérequis

Assurez-vous d'avoir:
- **NVIDIA CUDA Toolkit 11.0+** installé
- **NVIDIA Driver compatible** avec votre GPU
- **CMake 3.18+**

### 2️⃣ Vérifier Votre GPU

Ouvrez PowerShell et exécutez:
```powershell
nvidia-smi
```

**Exemple de sortie:**
```
NVIDIA GeForce RTX 3090
Compute Capability: 8.6
CUDA Version: 12.0
```

**Notez la valeur "Compute Capability"** (ex: 8.6) - vous en aurez besoin.

### 3️⃣ Configurer l'Architecture GPU Correcte

Éditez `CMakeLists.txt` ligne ~14:

```cmake
# Avant (valeur par défaut)
set(CMAKE_CUDA_ARCHITECTURES 75)
```

Remplacez par votre architecture:

| Compute Capability | Architecture | GPU Exemples |
|---|---|---|
| 7.0 | 70 | Tesla V100, RTX 2060 |
| 7.5 | 75 | RTX 2080, GTX 1650 |
| 8.0 | 80 | Tesla A100 |
| 8.6 | 86 | RTX 3090, RTX 3080, RTX 3070 |
| 8.9 | 89 | RTX 4090, RTX 4080 |
| 9.0 | 90 | H100 |

**Exemple pour RTX 3090:**
```cmake
set(CMAKE_CUDA_ARCHITECTURES 86)
```

### 4️⃣ Compiler le Projet

```powershell
cd d:\Slam_Project\extraction

# Nettoyer build précédent (optionnel)
rm -r build
mkdir build
cd build

# Configurer avec CMake (détecte automatiquement CUDA)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compiler
cmake --build . --config Release -j 4
```

### 5️⃣ Résoudre les Problèmes Courants

#### ❌ Erreur: "CUDA not found"
```
CMake Error at CMakeLists.txt:5 (enable_language):
  The CUDA compiler is not found.
```

**Solution:**
- Vérifier que CUDA Toolkit est installé
- Ajouter au PATH: `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.0\bin`
- Redémarrer PowerShell

#### ❌ Erreur: "Unsupported gpu architecture"
```
error: Unsupported gpu architecture "compute_86"
```

**Solution:**
- Vérifier que le driver NVIDIA est à jour: `nvidia-smi --driver-version`
- Mettre à jour CUDA Toolkit
- Vérifier l'architecture du GPU avec `nvidia-smi`

#### ❌ Erreur: "Cannot find -lcudart"
```
fatal error: undefined reference to 'cudaMalloc'
```

**Solution:**
- Vérifier que CMakeLists.txt contient: `target_link_libraries(...CUDA::cudart)`
- Reconfigurer CMake: `rm -r build && mkdir build && cd build && cmake ..`

### 6️⃣ Vérifier que CUDA Fonctionne

Après compilation, testez:

```powershell
./YourExecutable
```

Vous devriez voir dans les logs:
```
CUDA computation of distances completed successfully
```

---

## Configuration Avancée

### Multi-GPU Support (Optionnel)

Pour utiliser plusieurs GPUs:

```cpp
// Dans cuda_mapper_kernels.cu, avant cudaComputeDistances():
int device = 0;  // GPU 0
cudaSetDevice(device);
```

### Profiling CUDA (Optionnel)

Utiliser NVIDIA Nsight pour analyser les performances:

```powershell
# Avec Nsight Systems
nsys profile ./YourExecutable

# Avec Nsight Compute (pour analyse détaillée)
ncu --set full ./YourExecutable
```

### Désactiver CUDA pour Debugging

Si vous voulez tester le fallback CPU:

**Option 1 - Dans le code:**
```cpp
// Commenter dans Mapper.cpp
// ComputeDistancesGPU(...);  // Décommenter
ComputeDistancesCPU(...);     // Activer fallback
```

**Option 2 - Recompiler sans CUDA:**
```cmake
# Dans CMakeLists.txt, commenter:
# target_link_libraries(YourExecutable PRIVATE CUDA::cudart)
```

---

## Performance Tuning

### Ajuster la Taille des Blocs

Dans `cuda_mapper_kernels.cu` ligne ~108:

```cuda
int blockSize = 256;  // Valeurs testées: 128, 256, 512
```

**Recommandations:**
- GPU ancien (< 2060): 128
- GPU moderne (RTX 20/30): 256
- GPU haute-end (RTX 40/H100): 512

### Utiliser Unified Memory (Future Enhancement)

```cuda
// Au lieu de cudaMalloc/cudaMemcpy
cudaMallocManaged(&d_poses_x, size);  // Gestion auto CPU/GPU
```

---

## Vérification Finale

Créez un script de test `test_cuda.cpp`:

```cpp
#include "cuda_mapper_kernels.h"
#include <iostream>
#include <vector>

int main() {
    // Test avec 1000 poses
    int numPoses = 1000;
    std::vector<float> poses_x(numPoses, 1.5f);
    std::vector<float> poses_y(numPoses, 2.5f);
    std::vector<float> distances(numPoses);
    
    cudaError_t err = cudaComputeDistances(
        0.0f, 0.0f,
        poses_x.data(), poses_y.data(),
        numPoses, distances.data());
    
    if (err == cudaSuccess) {
        std::cout << "✓ CUDA OK! Distance[0] = " << distances[0] << std::endl;
        std::cout << "  Attendu: " << (1.5*1.5 + 2.5*2.5) << std::endl;
    } else {
        std::cout << "✗ CUDA Error: " << cudaGetErrorString(err) << std::endl;
    }
    
    return 0;
}
```

Compiler et tester:
```powershell
nvcc test_cuda.cpp cuda_mapper_kernels.cu -o test_cuda
./test_cuda
```

---

## Tableau de Performance Théorique

| Scans | Distance CPU | Distance GPU | Gain |
|---|---|---|---|
| 100 | 0.1 ms | 0.5 ms | 0.2× (overhead domine) |
| 500 | 0.5 ms | 0.6 ms | 0.8× |
| 1000 | 1.0 ms | 0.7 ms | **1.4×** ✓ |
| 5000 | 5.0 ms | 0.9 ms | **5.5×** ✓✓ |
| 10000 | 10.0 ms | 1.2 ms | **8.3×** ✓✓✓ |

**Conclusion:** CUDA est bénéfique pour **> 500 scans**

---

## Désinstaller / Réinitialiser

Pour revenir à une version sans CUDA:

```powershell
# Supprimer le dossier build
rm -r d:\Slam_Project\extraction\build

# Restaurer CMakeLists.txt original (sans CUDA)
# Restaurer Mapper.cpp sans GPU (utiliser version CPU uniquement)
```

---

## Ressources Utiles

- **NVIDIA CUDA Documentation:** https://docs.nvidia.com/cuda/
- **GPU Architecture Chart:** https://developer.nvidia.com/cuda-gpus
- **CMake + CUDA Guide:** https://cmake.org/cmake/help/latest/language/CUDA/
- **CUDA Best Practices:** https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/

---

**Status: ✓ Configuration CUDA complète et testée**
