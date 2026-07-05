#pragma once

//#include "../Laberinto/Laberinto.hpp"
#include "../Common/Common.hpp"
//#include "../UI/UI.hpp"

class Personaje {
protected:
	//std::shared_ptr<UI> ui;
	//std::shared_ptr<Laberinto> laberinto;
public:

	Posicion posicion;

	virtual ~Personaje() = default;

	// Depende de cada rol (heroe/stalker|local/remoto)
	virtual void mover() = 0;
};