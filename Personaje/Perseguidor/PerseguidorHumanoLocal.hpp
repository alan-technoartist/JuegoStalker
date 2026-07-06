#include "../Personaje.hpp"
#include "../../Red/ClienteRed.hpp"

class PerseguidorHumanoLocal : public Personaje {
private:
	std::shared_ptr<ClienteRed> red;

	void moverLocal(Direccion direccion);

public:

	PerseguidorHumanoLocal(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ClienteRed> red);

	void mover() override;

};