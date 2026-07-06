#pragma once
#include <string>

enum class Direccion {
	ARRIBA = 0,
	ABAJO,
	IZQUIERDA,
	DERECHA
};

enum class Tecla : unsigned int {
	ARRIBA = 0,
	ABAJO,
	IZQUIERDA,
	DERECHA,
	UNO,
	DOS,
	TRES,
	CUATRO,
	NADA
};

enum class TipoEntidad {
	HEROE = 0,
	PERSEGUIDOR,
	LLAVE,
	SALIDA
};

class UI {
public:
    virtual Tecla leerTeclado() = 0;
	virtual void desplegarTexto(std::string texto) = 0;

    virtual void dibujarPared(int x, int y) = 0;
    virtual void borrarCelda(int x, int y) = 0;
	virtual void actualizarEntidad(int x, int y, TipoEntidad entidad) = 0;
	virtual void render() = 0;
    virtual ~UI() = default;

};