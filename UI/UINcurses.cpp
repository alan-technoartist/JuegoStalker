#include "UINcurses.hpp"

UINcurses::UINcurses() {
    initscr();
    cbreak();             // Captura teclas inmediatamente sin esperar Enter
    noecho();
    keypad(stdscr, TRUE); // Habilita el uso de las flechas del teclado
    curs_set(0);

    clear(); // Borra la pantalla anterior
}
UINcurses::~UINcurses() {
    // Restauración obligatoria de la terminal antes de salir
    endwin();
}

Tecla UINcurses::leerTeclado() {
    int input = getch();
    Tecla tecla;

    switch (input) {
    case KEY_UP:
        tecla = Tecla::ARRIBA;
        break;
    case KEY_DOWN:
        tecla = Tecla::ABAJO;
        break;
    case KEY_LEFT:
        tecla = Tecla::IZQUIERDA;
        break;
    case KEY_RIGHT:
        tecla = Tecla::DERECHA;
        break;
    case '1':
        tecla = Tecla::UNO;
        break;
    case '2':
        tecla = Tecla::DOS;
        break;
    case '3':
        tecla = Tecla::TRES;
        break;
    case '4':
        tecla = Tecla::CUATRO;
        break;
    default:
        tecla = Tecla::NADA;
    }

    return tecla;
}

void UINcurses::desplegarTexto(std::string texto) {
    static int i = 0;
    mvprintw(21 /* + (++i)*/, 0, texto.c_str());
    refresh();
}

void UINcurses::dibujarPared(int x, int y) {
    mvaddch(x, y, ACS_BLOCK);
   // refresh();

}
void UINcurses::borrarCelda(int x, int y) {
    mvaddch(x, y, ' ');
    //refresh();
}

void UINcurses::actualizarEntidad(int x, int y, TipoEntidad entidad) {

    switch (entidad) {
    case TipoEntidad::HEROE:
        mvaddch(x, y, 'X');
        break;
    case TipoEntidad::PERSEGUIDOR:
        mvaddch(x, y, '@');
        break;
    case TipoEntidad::LLAVE:
        mvaddch(x, y, '*');
        break;
    case TipoEntidad::SALIDA:
        mvaddch(x, y, 'E');
        break;
    }

    //refresh();
}

void UINcurses::render() {
    refresh();
}

