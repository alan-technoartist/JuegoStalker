
#include "JuegoStalker.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    std::shared_ptr<UI> ui;

    std::shared_ptr<Laberinto> laberinto;
    std::shared_ptr<Personaje> heroe;
    std::shared_ptr<Personaje> perseguidor;


#ifdef CURSES
    //ui = std::make_shared<UINcurses>();
#elif QT
    //	ui = std::make_shared<UIQT>();
#else
    if (ui == nullptr) {
        std::cout << "Ninguna biblioteca de UI disponible" << std::endl;
        return -1;
    }
#endif

    int opcion;
    std::cout << "Selecciona opcion" << std::endl;
    std::cout << "1 - Crear partida nueva (solo)" << std::endl;
    std::cout << "2 - Crear partida nueva (multijugador)" << std::endl;
    std::cout << "3 - Unirse a una partida como perseguidor" << std::endl;
    std::cout << "4 - Salir" << std::endl;

    std::cin >> opcion;

    if (opcion == 1) {
        // Un solo jugador

        laberinto = std::make_shared<Laberinto>(ui);
        laberinto->dibujarLaberinto();

        // Local (teclado)
        heroe = std::make_shared<HeroeLocal>();

        // Perseguidor IA
        perseguidor = std::make_shared<PerseguidorIA>();

    }
    else if (opcion == 2) {
        // Multijugador (heroe)

        // Local (teclado)
        heroe = std::make_shared<HeroeLocal>();

        // Perseguidor humano remoto
        perseguidor = std::make_shared<PerseguidorHumanoRemoto>();

    }
    else if (opcion == 3) {
        // Multijugador (perseguidor)

        // Humano remoto (servidor)
        heroe = std::make_shared<HeroeRemoto>();

        // Humano local (cliente)
        perseguidor = std::make_shared<PerseguidorHumanoLocal>();

    }

    bool gameRunning = true;

	// Game loop
	while(gameRunning) {
        
	}

	return 0;
}
