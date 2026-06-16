#include "Laberinto.hpp"

Laberinto::Laberinto(std::shared_ptr<UI> ui) {
    this->ui = ui;

    for (int x = 0; x < LAB_HEIGHT; x++) {
        for (int y = 0; y < LAB_WIDTH; y++) {
            ui->dibujarPared(x, y);

            // Inincializar matriz de laberinto
            laberinto[x][y] = 1;
        }
    }
}

void Laberinto::dibujarCaminos(int x, int y) {
    // Marcar celda como visitada (camino libre)
    laberinto[x][y] = 0;
    ui->borrarCelda(x, y);

    std::array < int, 4 > movimientosX = { -2, 2, 0, 0 };
    std::array < int, 4 > movimientosY = { 0, 0, -2, 2 };
    std::array < int, 4 > direcciones = { 0, 1, 2, 3 };

    // Fuente de entropía
    std::random_device semilla;

    // Motor de generación (Mersenne Twister)
    std::mt19937 generador(semilla());

    std::shuffle(direcciones.begin(), direcciones.end(), generador);

    for (int direccion : direcciones) {
        // Decidir nuevos movimientos
        int nuevoX = x + movimientosX[direccion];
        int nuevoY = y + movimientosY[direccion];

        // Verificar límites del laberinto
        bool dentroLaberinto = nuevoX >= 0 &&
            nuevoY >= 0 &&
            nuevoX < LAB_WIDTH &&
            nuevoY < LAB_HEIGHT;

        // Si estamos dentro y si es pared, es un camino potencial
        if (dentroLaberinto && laberinto[nuevoX][nuevoY] == 1) {
            int xMedio = x + (movimientosX[direccion] / 2);
            int yMedio = y + (movimientosY[direccion] / 2);

            // "Derribar" pared intermedia
            laberinto[xMedio][yMedio] = 0;
            ui->borrarCelda(xMedio, yMedio);

            // Avanzar a la siguiente celda
            dibujarCaminos(nuevoX, nuevoY);
        }
    }

}

void Laberinto::crearCiclos(float porcentaje) {
    // Fuente de entropía
    std::random_device semilla;

    // Motor de generación (Mersenne Twister)
    std::mt19937 generador(semilla());

    // Distribución uniforme (rango: porcentaje)
    std::uniform_real_distribution distribucion(0.0, 1.0);

    for (int x = 1; x < LAB_WIDTH; x++) {
        for (int y = 1; y < LAB_HEIGHT; y++) {
            // Verificar límites del laberinto
            bool dentroLaberinto = x > 0 &&
                y > 0 &&
                x < LAB_WIDTH - 1 &&
                y < LAB_HEIGHT - 1;

            // Verificar si la celda es pared y estamos en el limite
            if (dentroLaberinto && laberinto[x][y] == 1 &&
                distribucion(generador) < porcentaje) {

                // "Derribar" pared
                laberinto[x][y] = 0;
                ui->borrarCelda(x, y);

            }
        }
    }
}

void Laberinto::dibujarLaberinto() {
    dibujarCaminos(1, 1);
    crearCiclos(0.1f);
}

bool Laberinto::esPared(int x, int y) {
    if (laberinto[x][y] == 1)
        return true;
    return false;
}