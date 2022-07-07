cmake_minimum_required(VERSION 3.0)
project(Breakout)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Breakout/dependencies/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_EXPORTER TRUE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
add_subdirectory(Breakout/dependencies/assimp)

file(COPY Breakout/dependencies/imgui/imgui.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/imconfig.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/imgui_internal.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/imstb_rectpack.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/imstb_textedit.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/imstb_truetype.h DESTINATION ../Breakout/Breakout/headers/imgui/)

file(COPY Breakout/dependencies/imgui/imgui.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)
file(COPY Breakout/dependencies/imgui/imgui_draw.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)
file(COPY Breakout/dependencies/imgui/imgui_tables.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)
file(COPY Breakout/dependencies/imgui/imgui_widgets.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)


file(COPY Breakout/dependencies/imgui/backends/imgui_impl_glfw.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/backends/imgui_impl_opengl3.h DESTINATION ../Breakout/Breakout/headers/imgui/)
file(COPY Breakout/dependencies/imgui/backends/imgui_impl_opengl3_loader.h DESTINATION ../Breakout/Breakout/headers/imgui/)

file(COPY Breakout/dependencies/imgui/backends/imgui_impl_glfw.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)
file(COPY Breakout/dependencies/imgui/backends/imgui_impl_opengl3.cpp DESTINATION ../Breakout/Breakout/sources/imgui/)


file(COPY Breakout/dependencies/tinyxml/tinyxml2.h DESTINATION ../Breakout/Breakout/headers/tinyxml/)
file(COPY Breakout/dependencies/tinyxml/tinyxml2.cpp DESTINATION ../Breakout/Breakout/sources/tinyxml/)

add_subdirectory(Breakout/Breakout)
