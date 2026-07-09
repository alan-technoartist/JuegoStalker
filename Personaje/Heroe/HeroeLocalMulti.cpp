
#include "HeroeLocalMulti.hpp"

HeroeLocalMulti::HeroeLocalMulti(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ServidorRed> red) :
	Personaje(posicionInicial, ui, laberinto) {
	this->red = red;

}

void HeroeLocalMulti::moverLocal(Direccion dir) {
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

void HeroeLocalMulti::mover() {

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

		// Mandar posición al perseguidor remoto
		red->enviarDatos(&posicion, sizeof(posicion));
	}
}

void HeroeLocalMulti::perder() {
	ui->desplegarTexto("========PERDISTE========");
}

void HeroeLocalMulti::ganar() {
	ui->desplegarTexto("========GANASTE========");
}