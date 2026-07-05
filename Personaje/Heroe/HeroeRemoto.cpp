#include "HeroeRemoto.hpp"
#include <iostream>

HeroeRemoto::HeroeRemoto(Posicion posicionInicial) {
    posicion = posicionInicial;

}

void HeroeRemoto::mover() {
    Posicion posicionHeroeRemoto;

    // Recibir posición del heroe remoto
    red.leerDatos(&posicionHeroeRemoto, sizeof(posicionHeroeRemoto));

    posicion = posicionHeroeRemoto;

}