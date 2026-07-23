#pragma once
#include <cstdint>
#include <random>
#include <iostream>
#include <chrono>

const int TARGET_FPS = 30;
const std::chrono::milliseconds duracionFrame(1000 / TARGET_FPS); // ~33ms por frame
const int FRAMES_ESPERA_STALKER = 15;

// Dimensiones del mapa del juego
const int LAB_WIDTH = 21;
const int LAB_HEIGHT = 21;
const int NUM_LLAVES = 3;

enum class Estado {
	JUGANDO = 0,
	GANADO,
	PERDIDO
};

struct Posicion {
	uint8_t  posicionX;
	uint8_t  posicionY;

	bool operator==(Posicion& o) {
		if ((o.posicionX == this->posicionX) &&
			(o.posicionY == this->posicionY)) {
			return true;
		}
		return false;
	}

	friend std::ostream& operator<<(std::ostream& os, const Posicion& pos) {
		os << "[" << static_cast<int>(pos.posicionX) << ","
			      << static_cast<int>(pos.posicionY) << "]";
		return os;
	}
};

struct Llave {
	Posicion posicion;
	bool recolectada;
};