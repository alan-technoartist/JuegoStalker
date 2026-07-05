
#include "PerseguidorHumanoLocal.hpp"

PerseguidorHumanoLocal::PerseguidorHumanoLocal(Posicion posicionInicial, std::shared_ptr<ClienteRed> red) {
	this->posicion = posicionInicial;
	this->red = red;
}

void PerseguidorHumanoLocal::moverLocal(Direccion dir) {
	/*switch (dir) {
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
	}*/
}

void PerseguidorHumanoLocal::mover() {

	int input = getch(); // Espera la entrada del usuario

	switch (input) {
	case KEY_UP:
		moverLocal(Direccion::ARRIBA);
		break;
	case KEY_DOWN:
		moverLocal(Direccion::ABAJO);
		break;
	case KEY_LEFT:
		moverLocal(Direccion::IZQUIERDA);
		break;
	case KEY_RIGHT:
		moverLocal(Direccion::DERECHA);
		break;
	}

	// Mandar posición al héroe remoto
	red->enviarDatos(&posicion, sizeof(posicion));
}

/*void HeroeLocal::inicializar() {
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
}*/