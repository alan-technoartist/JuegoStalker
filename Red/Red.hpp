#pragma once

class Red {
	// TODO: Implementar funciones base y quitarlas de las clases hijas
	virtual void enviarDatos(void* datos, int tamano) = 0;
	virtual void leerDatos(void* datos, int tamano) = 0;

	// Implementación depende del rol (cliente/servidor)
	virtual void inicializar() = 0;
};