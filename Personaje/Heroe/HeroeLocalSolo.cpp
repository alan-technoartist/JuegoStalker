
#include "HeroeLocalSolo.hpp"

HeroeLocalSolo::HeroeLocalSolo(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto) :
	Personaje(posicionInicial, ui, laberinto) {
}

void HeroeLocalSolo::moverLocal(Direccion dir) {
	switch (dir) {
	case Direccion::IZQUIERDA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY - 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else
				ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);

			posicion.posicionY -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);
		}
		break;
	case Direccion::DERECHA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY + 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else
				ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);

			posicion.posicionY += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);
		}
		break;
	case Direccion::ARRIBA:
		if (!(laberinto.esPared(posicion.posicionX - 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else
				ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);

			posicion.posicionX -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);
		}
		break;
	case Direccion::ABAJO:
		if (!(laberinto.esPared(posicion.posicionX + 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else
				ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);

			posicion.posicionX += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);
		}
		break;
	}
}

void HeroeLocalSolo::mover() {

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
	}
}

Posicion HeroeLocalSolo::obtenerPosicion() {
	return this->posicion;
}


void HeroeLocalSolo::perder() {
	ui->desplegarTexto("========PERDISTE========");
}

void HeroeLocalSolo::ganar() {
	ui->desplegarTexto("========GANASTE========");
}