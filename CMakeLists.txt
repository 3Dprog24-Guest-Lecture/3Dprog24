cmake_minimum_required(VERSION 3.8)

project("3Dprog24")

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

# Include directories
include_directories(
  core
  includes/imgui
  includes/glad
  includes/stbi
  libs/glm
  libs/assimp/include
)

# IMGUI headers and sources
set(IMGUI_HEADERS
  includes/imgui/imgui.h
  includes/imgui/imconfig.h
  includes/imgui/imgui_impl_opengl3.h
  includes/imgui/imgui_impl_glfw.h
  includes/imgui/imgui_impl_opengl3_loader.h
)

set(IMGUI_SOURCE
  includes/imgui/imgui.cpp
  includes/imgui/imgui_demo.cpp
  includes/imgui/imgui_draw.cpp
  includes/imgui/imgui_widgets.cpp
  includes/imgui/imgui_impl_opengl3.cpp
  includes/imgui/imgui_impl_glfw.cpp
  includes/imgui/imgui_tables.cpp
)

# Project header and source files
set(HEADER_FILES
  includes/stbi/stb_image.h
  core/ModelLoader/Shared.h
  core/Camera/Camera.h
  core/Window.h
  core/Application.h
  core/Material.h
  core/Mesh.h
  core/Renderer.h
  core/Scene.h
  core/Texture.h
  core/Actor.h
  core/Logger.h
  core/Types.h  
  core/Transform.h
  core/Tag.h
  core/Vertex.h
  core/SceneGraph.h
  core/Controller.h 
  core/Defines.h 
  core/Camera/CameraController.h 
  core/Component.h 
  core/Collision/Collision.h 
  core/Skybox/Skybox.h 
  core/Collision/AABB.h 
  core/ActorController.h
  core/ModelLoader/AssimpLoader.h 
  core/ModelLoader/AssimpUtility.h
  core/Lights/Lights.h 
  core/Lights/DirectionalLight.h 
  core/Lights/PointLight.h  
  core/Physics/PhysicsComponent.h
)

set(SRC_FILES
  3Dprog24.cpp
  includes/glad/glad.c
  core/Shader.cpp
  core/Camera/Camera.cpp
  core/Window.cpp
  core/Application.cpp
  core/Texture.cpp 
  core/Material.cpp     
  core/Mesh.cpp
  core/Scene.cpp
  core/Actor.cpp
  core/Camera/CameraController.cpp
  core/Physics/PhysicsComponent.cpp
  core/Skybox/Skybox.cpp  
  core/ModelLoader/AssimpLoader.cpp
  core/ActorController.cpp
)

# External libraries
add_subdirectory(libs/glfw-3.3.9)
add_subdirectory(libs/assimp)
add_subdirectory(libs/glm/glm)

# Create project executable
add_executable(${PROJECT_NAME} ${IMGUI_HEADERS} ${IMGUI_SOURCE} ${HEADER_FILES} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw assimp)

# MSVC specific settings
if(MSVC)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
