#include "../Personaje.hpp"

class PerseguidorIA : public Personaje {
public:

	PerseguidorIA(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto);
	~PerseguidorIA();

	void mover() override;
};