#include "../Personaje.hpp"
#include "../../Red/ClienteRed.hpp"

class HeroeRemoto: public Personaje {
private:
	std::shared_ptr<ClienteRed> red;

public:

	HeroeRemoto(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ClienteRed> red);

	void mover() override;

};