#pragma once

#include <memory>
#include "../Laberinto/Laberinto.hpp"
//#include "../UI/UI.hpp"

enum class Direccion {
	ARRIBA = 0,
	ABAJO,
	IZQUIERDA,
	DERECHA
};

struct Posicion {
	uint8_t  posicionX;
	uint8_t  posicionY;
};

class Personaje {
protected:
	//std::shared_ptr<UI> ui;
	std::shared_ptr<Laberinto> laberinto;
public:

	Posicion posicion;

	virtual ~Personaje() = default;

	virtual void mover() = 0;
};