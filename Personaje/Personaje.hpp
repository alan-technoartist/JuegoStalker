#include <memory>
#include <random>
#include "../UI/UI.hpp"
#include "../Laberinto/Laberinto.hpp"

enum class Direccion {
	ARRIBA = 0,
	ABAJO,
	IZQUIERDA,
	DERECHA
};

class Personaje {
public:
	int posicionX;
	int posicionY;

	std::shared_ptr<UI> ui;
	std::shared_ptr<Laberinto> laberinto;

	Personaje(std::shared_ptr<Laberinto> lab, std::shared_ptr<UI> ui);
	Personaje(std::shared_ptr<Laberinto> lab, std::shared_ptr<UI> ui, int posX, int posY);

	void mover(Direccion dir);
	void inicializar();
};