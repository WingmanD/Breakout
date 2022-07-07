cmake_minimum_required(VERSION 3.0)
project(Breakout)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(headers/
                    ../dependencies/assimp/include/
                    ../dependencies/glad/include/
                    ../dependencies/glfw/include/
                    ../dependencies/glm/
			        ../dependencies/stb/
                    ../dependencies/imgui/
                    ../dependencies/imgui/backends/
                    ../dependencies/tinyxml/
                    ../dependencies/SDL2-2.0.22/include/
			  ../dependencies/SDL2_mixer-2.0.4/include/)

file(GLOB DEPENDENCY_SOURCES ../dependencies/glad/src/glad.c)
file(GLOB_RECURSE PROJECT_HEADERS headers/*.hpp
                          headers/*.h)
file(GLOB_RECURSE PROJECT_SOURCES sources/*.cpp)
file(GLOB_RECURSE PROJECT_SHADERS shaders/*.comp
                          shaders/*.frag
                          shaders/*.geom
                          shaders/*.vert)
file(GLOB_RECURSE PROJECT_RESOURCES resources/*)
						  
link_directories(../dependencies/SDL2-2.0.22/lib/x64/
		     ../dependencies/SDL2_mixer-2.0.4/lib/x64/)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Dependencies" FILES ${DEPENDENCY_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})
set_source_files_properties(${PROJECT_RESOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_compile_definitions(NOMINMAX)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} 
                               ${DEPENDENCY_SOURCES} ${PROJECT_RESOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw SDL2.lib SDL2_mixer.lib
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
					  
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    DEPENDS ${PROJECT_SHADERS}
)
	
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${PROJECT_RESOURCES}
)

file(GLOB THIRD_PARTY_DLLS 
	${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/SDL2_mixer-2.0.4/lib/x64/*.dll
	${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/SDL2-2.0.22/lib/x64/SDL2.dll
)

foreach(CurrentHeaderFile IN LISTS THIRD_PARTY_DLLS)
	message(STATUS "creating copy command for file: ${CurrentHeaderFile}")
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		${CurrentHeaderFile} 
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/
		COMMENT "copying third party dll: ${CMAKE_CURRENT_SOURCE_DIR}${CurrentHeaderFile}"
	)
endforeach()

