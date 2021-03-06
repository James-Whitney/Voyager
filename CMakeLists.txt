set(VOYAGER_VERSION 0.1.0)

# CMake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(voyager VERSION ${VOYAGER_VERSION} LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 11)

###############################################################################################

function(AddExt TargetName)

   # glfw
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext/glfw/include")
   target_link_libraries(${TargetName} glfw ${GLFW_LIBRARIES})

   #bullet
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext/bullet/src")
   target_link_libraries(${TargetName}
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/Bullet3Collision/libBullet3Collision.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/Bullet3Common/libBullet3Common.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/Bullet3Dynamics/libBullet3Dynamics.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/Bullet3Geometry/libBullet3Geometry.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/Bullet3OpenCL/libBullet3OpenCL_clew.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/BulletCollision/libBulletCollision.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/BulletDynamics/libBulletDynamics.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/BulletInverseDynamics/libBulletInverseDynamics.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/BulletSoftBody/libBulletSoftBody.a"
   "${PROJECT_SOURCE_DIR}/ext/bullet/src/LinearMath/libLinearMath.a")

   # glad
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext/glad/include")

   # sfml
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext/sfml/include")
   target_link_libraries(${TargetName} "${PROJECT_SOURCE_DIR}/build/ext/sfml/lib/libsfml-audio.a" "${PROJECT_SOURCE_DIR}/build/ext/sfml/lib/libsfml-graphics.a" "${PROJECT_SOURCE_DIR}/build/ext/sfml/lib/libsfml-network.a" "${PROJECT_SOURCE_DIR}/build/ext/sfml/lib/libsfml-system.a" "${PROJECT_SOURCE_DIR}/build/ext/sfml/lib/libsfml-window.a")

   # general include dir
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext")

   # OS specific options and libraries
   if(WIN32)
      # c++0x is enabled by default.
      # -Wall produces way too many warnings.
      # -pedantic is not supported.
      target_link_libraries(${TargetName} opengl32.lib)
   else()
      # Enable all pedantic warnings.
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic")
      if(APPLE)
         # Add required frameworks for GLFW.
         target_link_libraries(${TargetName} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
      else()
         #Link the Linux OpenGL library
         target_link_libraries(${TargetName} "GL" "dl")
      endif()
   endif()

endfunction(AddExt)

function(AddRapidJson TargetName)
   target_include_directories(${TargetName} PUBLIC "${PROJECT_SOURCE_DIR}/ext/rapidjson/include")
endfunction(AddRapidJson)

function(LinkVoyagerLibraries TargetName)
   target_link_libraries(${TargetName} voyager-actors)
   target_link_libraries(${TargetName} voyager-core)
   target_link_libraries(${TargetName} voyager-loader)
   target_link_libraries(${TargetName} voyager-physics)
   target_link_libraries(${TargetName} voyager-render)
   target_link_libraries(${TargetName} voyager-utils)
   target_link_libraries(${TargetName} voyager-hud)
   target_link_libraries(${TargetName} voyager-vfc)
endfunction(LinkVoyagerLibraries)

function(ConfigureBuildProperties TargetName)
   target_include_directories(${TARGET_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
endfunction(ConfigureBuildProperties)

###############################################################################################

# GLFW
set(GLFW_DIR "${PROJECT_SOURCE_DIR}/ext/glfw")
option(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
option(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
option(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
add_subdirectory(${GLFW_DIR})

###############################################################################################

# SFML
set(SFML_DIR "${PROJECT_SOURCE_DIR}/ext/SFML")
option(SFML_BUILD_EXAMPLES "SFML_BUILD_EXAMPLES" OFF)
option(SFML_BUILD_TESTS "SFML_BUILD_TESTS" OFF)
option(SFML_BUILD_DOCS "SFML_BUILD_DOCS" OFF)
add_subdirectory(${SFML_DIR})

###############################################################################################

# Output Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Engine Libraries
add_subdirectory("src/voyager-actors")
add_subdirectory("src/voyager-core")
add_subdirectory("src/voyager-loader")
add_subdirectory("src/voyager-physics")
add_subdirectory("src/voyager-render")
add_subdirectory("src/voyager-utils")
add_subdirectory("src/voyager-hud")
add_subdirectory("src/voyager-vfc")

# Engine Executables
add_subdirectory("src/voyager-client")
add_subdirectory("src/voyager-server")
