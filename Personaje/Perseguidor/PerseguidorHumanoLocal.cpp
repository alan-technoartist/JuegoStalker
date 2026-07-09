#include "PerseguidorHumanoLocal.hpp"

PerseguidorHumanoLocal::PerseguidorHumanoLocal(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ClienteRed> red) :
	Personaje(posicionInicial, ui, laberinto) {
	this->red = red;
}

void PerseguidorHumanoLocal::moverLocal(Direccion dir) {
	switch (dir) {
	case Direccion::IZQUIERDA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY - 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else 
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionY -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::DERECHA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY + 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionY += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::ARRIBA:
		if (!(laberinto.esPared(posicion.posicionX - 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionX -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::ABAJO:
		if (!(laberinto.esPared(posicion.posicionX + 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionX += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	}
}

void PerseguidorHumanoLocal::mover() {

	Tecla tecla = ui->leerTeclado(); // Espera la entrada del usuario

	if (tecla != Tecla::NADA) {

		switch (tecla) {
		case Tecla::ARRIBA:
			moverLocal(Direccion::ARRIBA);
			break;
		case Tecla::ABAJO:
			moverLocal(Direccion::ABAJO);
			break;
		case Tecla::IZQUIERDA:
			moverLocal(Direccion::IZQUIERDA);
			break;
		case Tecla::DERECHA:
			moverLocal(Direccion::DERECHA);
			break;
		}

		// Mandar posición al héroe remoto
		red->enviarDatos(&posicion, sizeof(posicion));
	}
}

void PerseguidorHumanoLocal::perder() {
	ui->desplegarTexto("========PERDISTE========");
}

void PerseguidorHumanoLocal::ganar() {
	ui->desplegarTexto("========GANASTE========");
}