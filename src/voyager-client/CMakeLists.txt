
# What are we making?
set(TARGET_NAME voyager-client)
file(GLOB_RECURSE SOURCES "main.cpp")
add_executable(${TARGET_NAME} ${SOURCES})

# What does it depend on?
AddExt(${TARGET_NAME})
LinkVoyagerLibraries(${TARGET_NAME})

# How should we make it?
ConfigureBuildProperties(${TARGET_NAME})
