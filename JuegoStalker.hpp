#pragma once

#define CURSES

#include <thread>
#include "Common/Common.hpp"

#include "Laberinto/Laberinto.hpp"
#include "Personaje/Heroe/HeroeLocalSolo.hpp"
#include "Personaje/Heroe/HeroeLocalMulti.hpp"
#include "Personaje/Heroe/HeroeRemoto.hpp"
#include "Personaje/Perseguidor/PerseguidorHumanoLocal.hpp"
#include "Personaje/Perseguidor/PerseguidorHumanoRemoto.hpp"
#include "Personaje/Perseguidor/PerseguidorIA.hpp"

#include "Red/ServidorRed.hpp"
#include "Red/ClienteRed.hpp"
#include "UI/UI.hpp"

#ifdef CURSES
#include "UI/UINcurses.hpp"
#elif QT
//#include "UI/UIQT.hpp"
#endif

const int TARGET_FPS = 30;
const std::chrono::milliseconds duracionFrame(1000 / TARGET_FPS); // ~33ms por frame
const int FRAMES_ESPERA_STALKER = 15;
