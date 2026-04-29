cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Enable CUDA support
enable_language(CUDA)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Set CUDA architecture (adjust based on your GPU)
# For NVIDIA Jetson Nano: 53
# For Desktop GPUs:
# - 70: Tesla V100, RTX 2060
# - 75: RTX 2080, RTX 2060 Super
# - 86: RTX 3090, RTX 3080, RTX 3070
set(CMAKE_CUDA_ARCHITECTURES 53) # Jetson Nano (Maxwell architecture)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Add all your source files including CUDA kernels
set(SOURCES
    src/Fonction_Test.cpp
    src/Karto.cpp
    src/Mapper.cpp
    src/cuda_mapper_kernels.cu
    main.cpp
)

# Add executable
add_executable(YourExecutable ${SOURCES})

# Link CUDA runtime
target_link_libraries(YourExecutable PRIVATE CUDA::cudart)

find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(YourExecutable PRIVATE Boost::serialization)

find_package(TBB REQUIRED)
target_link_libraries(YourExecutable PRIVATE TBB::tbb)