#pragma once

#include <array>
#include "../Common/Common.hpp"
#include "../UI/UI.hpp"

// Dimensiones del mapa del juego
const int LAB_WIDTH = 21;
const int LAB_HEIGHT = 21;
const int NUM_LLAVES = 3;

class Laberinto {
private:
    // Matriz que representa el laberinto
    std::array < std::array<int, LAB_HEIGHT>, LAB_WIDTH> laberinto;

    // Motor de generación (Mersenne Twister)
    std::mt19937 generador;
    uint32_t semillaInicial;

    void dibujarCaminos(int x, int y);
    void crearCiclos(float porcentaje);

    std::shared_ptr<UI> ui;

public:
    Laberinto(std::shared_ptr<UI> ui);


    void iniciarMotor(uint32_t semillaInicial);
    void generar();

    // Regresa casilla sin pared (no garantiza libre de otros actores)
    Posicion obtenerCasillaLibre();

    bool esPared(int x, int y);
};