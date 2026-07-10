#include <future>
#include <onnxruntime_cxx_api.h>

#include "../Personaje.hpp"

class PerseguidorIA : public Personaje {
private:
	void construirEntrada(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 >& datosEntrada);
	Direccion ejecutarInferencia(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 > datosEntrada);

	void moverLocal(Direccion dir);

	Posicion posicionHeroe;

public:

	PerseguidorIA(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, Posicion posicionHeroe);
	~PerseguidorIA();


	void mover() override;
	void perder() override;
	void ganar() override;

};