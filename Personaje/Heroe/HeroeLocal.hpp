#include "../Personaje.hpp"
#include "../../UI/UINcurses.hpp"

class HeroeLocal : public Personaje {
private:
	void moverLocal(Direccion direccion);

public:

	HeroeLocal(Posicion posicionInicial);

	void mover() override;

};