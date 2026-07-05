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

# Add source to this project's executable.
add_executable (JuegoStalker
	"JuegoStalker.cpp"
	"JuegoStalker.hpp"
	"Laberinto/Laberinto.cpp"
	"UI/UINcurses.cpp"
	"Personaje/Heroe/HeroeLocal.cpp"
	"Red/ServidorRed.cpp"
	"Red/ClienteRed.cpp"
 "Personaje/Heroe/HeroeRemoto.cpp" "Personaje/Perseguidor/PerseguidorHumanoLocal.cpp" "Personaje/Perseguidor/PerseguidorHumanoRemoto.cpp" "Personaje/Perseguidor/PerseguidorIA.cpp" "Red/RedBase.cpp")

target_link_libraries(JuegoStalker PRIVATE unofficial::pdcurses::pdcurses)
target_link_libraries(JuegoStalker PRIVATE Boost::headers)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET JuegoStalker PROPERTY CXX_STANDARD 20)
endif()
