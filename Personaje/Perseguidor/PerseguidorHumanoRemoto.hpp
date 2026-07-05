#include "../Personaje.hpp"
#include "../../Red/ServidorRed.hpp"

class PerseguidorHumanoRemoto : public Personaje {
private:
	std::shared_ptr<ServidorRed> red;

public:
	PerseguidorHumanoRemoto(Posicion posicionInicial, std::shared_ptr<ServidorRed> red);

	void mover() override;

};