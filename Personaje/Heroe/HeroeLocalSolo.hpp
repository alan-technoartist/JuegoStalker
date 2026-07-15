#pragma once
#include "../Personaje.hpp"

class HeroeLocalSolo : public Personaje {
private:
	void moverLocal(Direccion direccion);

public:

	HeroeLocalSolo(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto);

	Posicion obtenerPosicion();
	void mover() override;
	void perder() override;
	void ganar() override;
};