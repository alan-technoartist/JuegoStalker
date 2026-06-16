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

void UINcurses::dibujarPared(int x, int y) {
    mvaddch(x, y, ACS_BLOCK);
    refresh();

}
void UINcurses::borrarCelda(int x, int y) {
    mvaddch(x, y, ' ');
    refresh();

}

void UINcurses::dibujarPersonaje(int x, int y) {
    mvaddch(x, y, 'X');
    refresh();
}