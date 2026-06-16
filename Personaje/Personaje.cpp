
#include "Personaje.hpp"

Personaje::Personaje(std::shared_ptr<Laberinto> lab, std::shared_ptr<UI> ui) :
	posicionX(1), posicionY(1)
{
	this->ui = ui;
	this->laberinto = lab;
}

Personaje::Personaje(std::shared_ptr<Laberinto> lab, std::shared_ptr<UI> ui, int x, int y) :
	posicionX(x), posicionY(y)
{
	this->ui = ui;
	this->laberinto = lab;
}

void Personaje::mover(Direccion dir) {
	switch (dir) {
	case Direccion::IZQUIERDA:
		if (!(laberinto->esPared(posicionX, posicionY - 1))) {
			ui->borrarCelda(posicionX, posicionY);
			posicionY -= 1;
			ui->dibujarPersonaje(posicionX, posicionY);
		}
		break;
	case Direccion::DERECHA:
		if (!(laberinto->esPared(posicionX, posicionY + 1))) {
			ui->borrarCelda(posicionX, posicionY);
			posicionY += 1;
			ui->dibujarPersonaje(posicionX, posicionY);
		}
		break;
	case Direccion::ARRIBA:
		if (!(laberinto->esPared(posicionX -1, posicionY))) {
			ui->borrarCelda(posicionX, posicionY);
			posicionX -= 1;
			ui->dibujarPersonaje(posicionX, posicionY);
		}
		break;
	case Direccion::ABAJO:
		if (!(laberinto->esPared(posicionX + 1, posicionY))) {
			ui->borrarCelda(posicionX, posicionY);
			posicionX += 1;
			ui->dibujarPersonaje(posicionX, posicionY);
		}
		break;
	}
}

void Personaje::inicializar() {
	// Fuente de entropía
	std::random_device semilla;

	// Motor de generación (Mersenne Twister)
	std::mt19937 generador(semilla());

	// Distribución uniforme (rango: porcentaje)
	std::uniform_int_distribution distribucionX(1, LAB_WIDTH - 1);
	std::uniform_int_distribution distribucionY(1, LAB_HEIGHT - 1);

	int posInicialX, posInicialY;

	do {
		posInicialX = distribucionX(generador);
		posInicialY = distribucionY(generador);

	} while (laberinto->esPared(posInicialX, posInicialY));

	// Asignar posicion inicial del personaje
	posicionX = posInicialX;
	posicionY = posInicialY;

	ui->dibujarPersonaje(posicionX, posicionY);
}