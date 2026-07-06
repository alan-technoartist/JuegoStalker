#include "../Personaje.hpp"
#include "../../Red/ServidorRed.hpp"

class HeroeLocalMulti : public Personaje {
private:
	std::shared_ptr<ServidorRed> red;

	void moverLocal(Direccion direccion);

public:

	HeroeLocalMulti(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ServidorRed> red);

	void mover() override;

};