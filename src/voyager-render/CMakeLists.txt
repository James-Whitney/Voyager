# What are we making?
set(TARGET_NAME voyager-render)
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "*.cpp")
add_library(${TARGET_NAME} ${HEADERS} ${SOURCES})

# What does it depend on?
AddExt(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} voyager-core)
target_link_libraries(${TARGET_NAME} voyager-utils)
target_link_libraries(${TARGET_NAME} voyager-hud)

# How should we make it?
target_include_directories(${TARGET_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
