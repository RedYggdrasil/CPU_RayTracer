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
  ${APP_PUBLIC_SRC}/Defines/Defines.h					${APP_PRIVTE_SRC}/Defines/Defines.cpp
  ${APP_PUBLIC_SRC}/Defines/RefractionIndexes.h			${APP_PRIVTE_SRC}/Defines/RefractionIndexes.cpp
  ${APP_PUBLIC_SRC}/Debug/DebugMath.h					${APP_PRIVTE_SRC}/Debug/DebugMath.cpp
  ${APP_PUBLIC_SRC}/Debug/DebugReference.h				${APP_PRIVTE_SRC}/Debug/DebugReference.cpp
  ${APP_PUBLIC_SRC}/Hittables/Hittable.h				${APP_PRIVTE_SRC}/Hittables/Hittable.cpp
  ${APP_PUBLIC_SRC}/Hittables/HList.h					${APP_PRIVTE_SRC}/Hittables/HList.cpp
  ${APP_PUBLIC_SRC}/Hittables/HSphere.h					${APP_PRIVTE_SRC}/Hittables/HSphere.cpp
  ${APP_PUBLIC_SRC}/Maths/Camera.h						${APP_PRIVTE_SRC}/Maths/Camera.cpp
  ${APP_PUBLIC_SRC}/Maths/Interval.h					${APP_PRIVTE_SRC}/Maths/Interval.cpp
  ${APP_PUBLIC_SRC}/Maths/Ray.h							${APP_PRIVTE_SRC}/Maths/Ray.cpp
  ${APP_PUBLIC_SRC}/Maths/RMath.h						${APP_PRIVTE_SRC}/Maths/RMath.cpp
  ${APP_PUBLIC_SRC}/Maths/RMathCol.h					${APP_PRIVTE_SRC}/Maths/RMathCol.cpp
  ${APP_PUBLIC_SRC}/Maths/RMathRand.h					${APP_PRIVTE_SRC}/Maths/RMathRand.cpp
  ${APP_PUBLIC_SRC}/Maths/RMathGeo.h					${APP_PRIVTE_SRC}/Maths/RMathGeo.cpp
  ${APP_PUBLIC_SRC}/Assets/Materials/Material.h			${APP_PRIVTE_SRC}/Assets/Materials/Material.cpp
  ${APP_PUBLIC_SRC}/Assets/Materials/LambertianMat.h	${APP_PRIVTE_SRC}/Assets/Materials/LambertianMat.cpp 
  ${APP_PUBLIC_SRC}/Assets/Materials/MetalMat.h			${APP_PRIVTE_SRC}/Assets/Materials/MetalMat.cpp 
  ${APP_PUBLIC_SRC}/Assets/Materials/DielectricMat.h	${APP_PRIVTE_SRC}/Assets/Materials/DielectricMat.cpp 
  ${APP_PUBLIC_SRC}/Assets/Scenes/Scene.h				${APP_PRIVTE_SRC}/Assets/Scenes/Scene.cpp
  ${APP_PUBLIC_SRC}/Assets/Scenes/FOVScene.h			${APP_PRIVTE_SRC}/Assets/Scenes/FOVScene.cpp
  ${APP_PUBLIC_SRC}/Assets/Scenes/GlassMetalDemoScene.h	${APP_PRIVTE_SRC}/Assets/Scenes/GlassMetalDemoScene.cpp
  ${APP_PUBLIC_SRC}/Assets/Scenes/GlassMetalDemoSceneLookAt.h		${APP_PRIVTE_SRC}/Assets/Scenes/GlassMetalDemoSceneLookAt.cpp
  ${APP_PUBLIC_SRC}/SystemElement/Picture.h				${APP_PRIVTE_SRC}/SystemElement/Picture.cpp
  ${APP_PUBLIC_SRC}/Tools/RCollection.h					${APP_PRIVTE_SRC}/Tools/RCollection.cpp
  ${APP_PUBLIC_SRC}/Tools/REnum.h						${APP_PRIVTE_SRC}/Tools/REnum.cpp
  ${APP_PUBLIC_SRC}/Tools/RLog.h						${APP_PRIVTE_SRC}/Tools/RLog.cpp
  ${APP_PUBLIC_SRC}/Tools/RRandomizer.h					${APP_PRIVTE_SRC}/Tools/RRandomizer.cpp
  ${APP_PUBLIC_SRC}/Tools/RRenderRandomizer.h			${APP_PRIVTE_SRC}/Tools/RRenderRandomizer.cpp
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

get_property(TRACY_INC_PATH GLOBAL PROPERTY TRACY_INC_PATH)

message("App name: '${APP_NAME}'")

message("Got tracy path: '${TRACY_INC_PATH}'")

target_link_libraries(${APP_NAME} PUBLIC Tracy::TracyClient)

target_include_directories(${APP_NAME} 
 PUBLIC
  "${PROJECT_BINARY_DIR}/${APP_NAME}/Gen_SRC/Public/"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/"
  ${TRACY_INC_PATH}
 PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/"
)