#include "HeroeRemoto.hpp"

HeroeRemoto::HeroeRemoto(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ClienteRed> red) :
    Personaje(posicionInicial, ui, laberinto) {
    this->red = red;
}

void HeroeRemoto::mover() {
    Posicion posicionHeroeRemoto;

    // Recibir posición del heroe remoto
    red->leerDatos(&posicionHeroeRemoto, sizeof(posicionHeroeRemoto));

    // Héroe sólo debe borrar llaves, no salida
    if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY))
        ui->borrarCelda(posicion.posicionX, posicion.posicionY);
    else
        ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);

    // Actualizar posición
    posicion = posicionHeroeRemoto;
    ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);

}

void HeroeRemoto::perder() {

}

void HeroeRemoto::ganar() {

}