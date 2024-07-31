cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Add all your source files
set(SOURCES 
    src/Fonction_Test.cpp
    src/Karto.cpp
    src/Mapper.cpp
    main.cpp
)

# Add executable
add_executable(YourExecutable ${SOURCES})


find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries(YourExecutable PRIVATE Boost::serialization)

find_package(TBB REQUIRED)
target_link_libraries(YourExecutable PRIVATE TBB::tbb)