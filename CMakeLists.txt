# CMakeList.txt : CMake project for JuegoStalker, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("JuegoStalker")

find_package(unofficial-pdcurses REQUIRED)
find_package(Boost REQUIRED)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_HTML_COLORSTYLE "LIGHT")

	set(DOXYGEN_EXTRACT_ALL "YES")
	set(DOXYGEN_EXTRACT_PRIVATE "YES")
	set(DOXYGEN_EXTRACT_STATIC "YES")

	set(DOXYGEN_HAVE_DOT "YES")
	set(DOXYGEN_CLASS_GRAPH "YES")
    set(DOXYGEN_COLLABORATION_GRAPH "YES")
    set(DOXYGEN_CALL_GRAPH "YES")
    set(DOXYGEN_CALLER_GRAPH "YES")
	set(DOXYGEN_UML_LOOK "YES")

	set(DOXYGEN_EXCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime")

	doxygen_add_docs(
		docs
		${CMAKE_CURRENT_SOURCE_DIR}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		ALL
	)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
# Habilitar herramientas de meta-compilación de Qt
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add source to this project's executable.
add_executable (JuegoStalker
	"JuegoStalker.cpp"
	"JuegoStalker.hpp"
	"Laberinto/Laberinto.cpp"
	"UI/UINcurses.cpp"
	"UI/UIQT.cpp"
	"Personaje/Heroe/HeroeLocalSolo.cpp"
	"Red/ServidorRed.cpp"
	"Red/ClienteRed.cpp"
 "Personaje/Heroe/HeroeRemoto.cpp" "Personaje/Perseguidor/PerseguidorHumanoLocal.cpp" "Personaje/Perseguidor/PerseguidorHumanoRemoto.cpp" "Personaje/Perseguidor/PerseguidorIA.cpp" "Red/RedBase.cpp" "Personaje/Heroe/HeroeLocalMulti.cpp")

set(ONNX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime/include")
set(ONNX_LIB "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime/onnxruntime.lib")
set(ONNX_DLL "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime/onnxruntime.dll")

target_include_directories(JuegoStalker PRIVATE ${ONNX_INCLUDE_DIR})
target_link_libraries(JuegoStalker PRIVATE ${ONNX_LIB})

add_custom_command(TARGET JuegoStalker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ONNX_DLL}"
    $<TARGET_FILE_DIR:JuegoStalker>
)

target_link_libraries(JuegoStalker PRIVATE unofficial::pdcurses::pdcurses)
target_link_libraries(JuegoStalker PRIVATE Boost::headers)
target_link_libraries(JuegoStalker PRIVATE Qt6::Core Qt6::Widgets)

# 1. Le pedimos a CMake que averigüe dónde está la carpeta "bin" de tu Qt
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
# 2. Buscamos el programa windeployqt en esa carpeta
find_program(WINDEPLOYQT_EXECUTABLE windeployqt6 PATHS "${_qt_bin_dir}")

# 3. Creamos el evento "Post-Build" automático
if(WINDEPLOYQT_EXECUTABLE)
    add_custom_command(TARGET JuegoStalker POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:JuegoStalker>
        COMMENT "Ejecutando windeployqt para copiar las DLLs de Qt..."
    )
else()
    message(WARNING "No se encontró windeployqt. Las DLLs no se copiarán.")
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET JuegoStalker PROPERTY CXX_STANDARD 20)
endif()
