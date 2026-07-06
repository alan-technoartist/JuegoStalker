#pragma once
#include <cstdint>
#include <random>
#include <iostream>

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