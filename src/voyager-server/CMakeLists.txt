
# What are we making?
set(TARGET_NAME voyager-server)
add_executable(${TARGET_NAME} main.cpp)

# What does it depend on?
LinkVoyagerLibraries(${TARGET_NAME})

# How should we make it?
ConfigureBuildProperties(${TARGET_NAME})
