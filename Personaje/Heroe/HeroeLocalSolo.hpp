#include "../Personaje.hpp"

class HeroeLocalSolo : public Personaje {
private:
	void moverLocal(Direccion direccion);

public:

	HeroeLocalSolo(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto);

	void mover() override;

};