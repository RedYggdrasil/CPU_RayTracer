# CMakeList.txt : CMake project for App, include source and define
# project specific logic here.
#
#project("App" VERSION 1.0)

set(APP_NAME "App")

set (APP_PUBLIC_SRC "Source/Public/${APP_NAME}/")
set (APP_PRIVTE_SRC "Source/Private/${APP_NAME}/")

# List of included sources
set(APP_SOURCES
  ${APP_PUBLIC_SRC}pch.h
  ${APP_PRIVTE_SRC}main.cpp
)

create_non_existing("${APP_SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}" ${APP_NAME})

# Add source to this project's executable.
add_executable ( ${APP_NAME} ${APP_SOURCES} )


# 'source_group' Ensure than Visual Studio Solution respect File Hierarchy in filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${APP_SOURCES})

configure_file("CMake_Source/Public/Gen_App/Config/AppConfig.h.in" "Gen_SRC/Public/Gen_App/Config/AppConfig.h")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 20)
endif()


target_include_directories(${APP_NAME} 
 PUBLIC
  "${PROJECT_BINARY_DIR}/${APP_NAME}/Gen_SRC/Public/"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/"
 PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/"
)