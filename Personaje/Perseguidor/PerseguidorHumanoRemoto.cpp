#include "PerseguidorHumanoRemoto.hpp"

PerseguidorHumanoRemoto::PerseguidorHumanoRemoto(Posicion posicionInicial, std::shared_ptr<ServidorRed> red) {
	this->posicion = posicionInicial;
    this->red = red;
}

void PerseguidorHumanoRemoto::mover() {
    Posicion posicionPerseguidorRemoto;

    // Recibir posición del perseguidor remoto
    red->leerDatos(&posicionPerseguidorRemoto, sizeof(posicionPerseguidorRemoto));

    posicion = posicionPerseguidorRemoto;

}