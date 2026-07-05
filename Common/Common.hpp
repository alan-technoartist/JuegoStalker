#pragma once
#include <cstdint>
#include <random>

enum class Direccion {
	ARRIBA = 0,
	ABAJO,
	IZQUIERDA,
	DERECHA
};

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
};

struct Llave {
	Posicion posicion;
	bool recolectada;
};