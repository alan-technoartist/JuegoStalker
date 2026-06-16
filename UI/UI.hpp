#pragma once

class UI {
public:
    virtual void dibujarPared(int x, int y) = 0;
    virtual void borrarCelda(int x, int y) = 0;
    virtual void dibujarPersonaje(int x, int y) = 0;
    virtual ~UI() = default;
};