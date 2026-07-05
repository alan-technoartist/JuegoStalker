#pragma once
#define CURSES

#include "Common/Common.hpp"

#include "Laberinto/Laberinto.hpp"
#include "Personaje/Heroe/HeroeLocal.hpp"
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
