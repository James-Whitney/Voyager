# What are we making?
set(TARGET_NAME voyager-hud)
file(GLOB_RECURSE HEADERS "include/*.h" "${PROJECT_SOURCE_DIR}/ext/imgui/*.h")
file(GLOB_RECURSE SOURCES "*.cpp" "${PROJECT_SOURCE_DIR}/ext/imgui/*.cpp")
add_library(${TARGET_NAME} ${HEADERS} ${SOURCES})

# What does it depend on?
AddExt(${TARGET_NAME})
AddRapidJson(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} voyager-utils)

# How should we make it?
target_include_directories(${TARGET_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
