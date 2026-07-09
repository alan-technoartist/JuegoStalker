#include "PerseguidorHumanoRemoto.hpp"

PerseguidorHumanoRemoto::PerseguidorHumanoRemoto(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ServidorRed> red) :
    Personaje(posicionInicial, ui, laberinto) {
    this->red = red;
}

void PerseguidorHumanoRemoto::mover() {
    Posicion posicionPerseguidorRemoto;

    // Recibir posición del perseguidor remoto
    red->leerDatos(&posicionPerseguidorRemoto, sizeof(posicionPerseguidorRemoto));

    // Perseguidor no debe borrar otras entidades
	if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
		!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
		ui->borrarCelda(posicion.posicionX, posicion.posicionY);
	else {
		if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
		else
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
	}

	// Actualizar posición
    posicion = posicionPerseguidorRemoto;
    ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);

}

void PerseguidorHumanoRemoto::perder() {

}

void PerseguidorHumanoRemoto::ganar() {

}