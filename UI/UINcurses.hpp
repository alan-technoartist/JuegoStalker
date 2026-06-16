#include <curses.h>
#include "UI.hpp"

class UINcurses : public UI {
public:
    UINcurses();
    ~UINcurses();

    void dibujarPared(int x, int y) override;
    void borrarCelda(int x, int y) override;
    void dibujarPersonaje(int x, int y) override;
};