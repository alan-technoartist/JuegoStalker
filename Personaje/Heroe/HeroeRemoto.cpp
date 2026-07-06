#include "HeroeRemoto.hpp"

HeroeRemoto::HeroeRemoto(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<ClienteRed> red) :
    Personaje(posicionInicial, ui, laberinto) {
    this->red = red;
}

void HeroeRemoto::mover() {
    Posicion posicionHeroeRemoto;

    // Recibir posición del heroe remoto
    red->leerDatos(&posicionHeroeRemoto, sizeof(posicionHeroeRemoto));

    ui->borrarCelda(posicion.posicionX, posicion.posicionY);

    posicion = posicionHeroeRemoto;

    ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::HEROE);

}