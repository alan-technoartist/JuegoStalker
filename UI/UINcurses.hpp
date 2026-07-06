#pragma once

#include <curses.h>
#include "UI.hpp"

class UINcurses : public UI {
public:
    UINcurses();
    ~UINcurses();

    Tecla leerTeclado() override;
    void desplegarTexto(std::string texto) override;
    void dibujarPared(int x, int y) override;
    void borrarCelda(int x, int y) override;
    void actualizarEntidad(int x, int y, TipoEntidad entidad);

    void render() override;

};