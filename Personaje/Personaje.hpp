#pragma once

#include "../Laberinto/Laberinto.hpp"
#include "../Common/Common.hpp"
#include "../UI/UI.hpp"

class Personaje {
protected:
	std::shared_ptr<UI> ui;
	Laberinto& laberinto;

public:
	Posicion posicion;
	
	Personaje(Posicion pos, std::shared_ptr<UI> uiPtr, Laberinto& lab)
		: ui(uiPtr), laberinto(lab), posicion(pos) {
	}

	virtual ~Personaje() = default;

	// Depende de cada rol (heroe/stalker|local/remoto)
	virtual void mover() = 0;
};