#include "PerseguidorHumanoRemoto.hpp"

PerseguidorHumanoRemoto::PerseguidorHumanoRemoto(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ServidorRed> red) :
    Personaje(posicionInicial, ui, laberinto) {
    this->red = red;
}

void PerseguidorHumanoRemoto::mover() {
    Posicion posicionPerseguidorRemoto;

    // Recibir posición del perseguidor remoto
    red->leerDatos(&posicionPerseguidorRemoto, sizeof(posicionPerseguidorRemoto));

    ui->borrarCelda(posicion.posicionX, posicion.posicionY);

    posicion = posicionPerseguidorRemoto;

    ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);

}