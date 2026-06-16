
#include "JuegoStalker.hpp"

int main()
{
	std::shared_ptr<UI> ui;

#ifdef CURSES
	ui = std::make_shared<UINcurses>();
#elif QT
//	ui = std::make_shared<UIQT>();
#else
	if (ui == nullptr) {
		std::cout << "Ninguna biblioteca de UI disponible" << std::endl;
		return -1;
	}
#endif

	std::shared_ptr<Laberinto> laberinto = std::make_shared<Laberinto>(ui);
	laberinto->dibujarLaberinto();

    std::shared_ptr<Personaje> personaje = std::make_shared<Personaje>(laberinto, ui);
    personaje->inicializar();

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
