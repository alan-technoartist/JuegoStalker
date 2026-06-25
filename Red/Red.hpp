
class Red {
	void enviarDatos(void* data);
	void leerDatos(void* buf);

	// Implementación depende del rol (cliente/servidor)
	virtual void inicializar() = 0;
};