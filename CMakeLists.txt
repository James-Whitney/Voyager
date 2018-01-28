set(VOYAGER_VERSION 0.1.0)

# CMake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(voyager VERSION ${VOYAGER_VERSION} LANGUAGES CXX)

function(LinkVoyagerLibraries TargetName)
   target_link_libraries(${TargetName} voyager-core)
endfunction(LinkVoyagerLibraries)

function(ConfigureBuildProperties TargetName)
   target_include_directories(${TARGET_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
endfunction(ConfigureBuildProperties)

# Output Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Engine Libraries
add_subdirectory("src/voyager-core")
add_subdirectory("src/voyager-utils")

# Engine Executables
add_subdirectory("src/voyager-client")
add_subdirectory("src/voyager-server")
