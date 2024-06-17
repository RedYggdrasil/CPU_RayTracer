cmake_minimum_required (VERSION 3.8)

project("CPU_RayTracer"  VERSION 1.0)

# Include sub-projects.


# Include sub-projects.
# The App
add_subdirectory ("CMakeLibs")

# The App
add_subdirectory ("App")


 # The Locally build libraries
  add_subdirectory("LLibs")


# Set app as the Startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "App")
