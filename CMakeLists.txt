cmake_minimum_required (VERSION 3.8)

project("CPU_RayTracer"  VERSION 1.0)


option( TRACY_ENABLE "" ON)


# Include sub-projects.
add_subdirectory ("CMakeLibs")


# The External build libraries
add_subdirectory("ELibs")

 # The Locally build libraries
  add_subdirectory("LLibs")

# The App
add_subdirectory ("App")




# Set app as the Startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "App")
