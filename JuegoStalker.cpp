
#include "JuegoStalker.hpp"

#include <iostream>

struct EstadoInicial {
    struct Personaje {
        int posicionX;
        int posicionY;
    };
    struct Perseguidor {
        int posicionX;
        int posicionY;
    };
    std::random_device semilla;
};

int main(int argc, char* argv[])
{
    std::shared_ptr<UI> ui;

    std::shared_ptr<Laberinto> laberinto;
    std::shared_ptr<Personaje> personaje;
    //std::shared_ptr<Perseguidor> perseguidor;


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

        personaje = std::make_shared<HeroeLocal>(laberinto, ui);
        personaje->inicializar();

        //perseguidor = std::make_shared<Perseguidor>(laberinto, ui);
        //perseguidor->inicializar();

    }
    else if (opcion == 2) {
        // Multijugador (heroe)



    }
    else if (opcion == 3) {
        // Multijugador (perseguidor)



    }

    bool gameRunning = true;

	// Game loop
	while(gameRunning) {
        int input = getch(); // Espera la entrada del usuario

        switch (input) {
        case KEY_UP:
            personaje->mover(Direccion::ARRIBA);
            break;
        case KEY_DOWN:
            personaje->mover(Direccion::ABAJO);
            break;
        case KEY_LEFT:
            personaje->mover(Direccion::IZQUIERDA);
            break;
        case KEY_RIGHT:
            personaje->mover(Direccion::DERECHA);
            break;
        case 'q': case 'Q':
            gameRunning = false;
            break;
        }
	}



	return 0;
}
