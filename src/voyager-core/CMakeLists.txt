
# What are we making?
set(TARGET_NAME voyager-core)
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "*.cpp" "${PROJECT_SOURCE_DIR}/ext/glad/src/glad.c")
add_library(${TARGET_NAME} ${HEADERS} ${SOURCES})

# What does it depend on?
AddExt(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} voyager-utils)

# How should we make it?
target_include_directories(${TARGET_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
