#include "../Personaje.hpp"
#include "../../UI/UINcurses.hpp"
#include "../../Red/ClienteRed.hpp"

class PerseguidorHumanoLocal : public Personaje {
private:
	std::shared_ptr<ClienteRed> red;

	void moverLocal(Direccion direccion);

public:
	std::random_device semillaInicial;

	PerseguidorHumanoLocal(Posicion posicionInicial, std::shared_ptr<ClienteRed> red);

	void mover() override;

};