#ifdef ML
#include <future>
#include <onnxruntime_cxx_api.h>
#endif

#include <queue>

#include "../Personaje.hpp"
#include "../Heroe/HeroeLocalSolo.hpp"

class PerseguidorIA : public Personaje {
private:

#ifdef ML
	void construirEntrada(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 >& datosEntrada);
	Direccion ejecutarInferencia(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 > datosEntrada);
	std::unique_ptr<Ort::Env> env;
	std::unique_ptr<Ort::Session> session;
	Ort::MemoryInfo memory_info{ nullptr };
#endif
	Direccion calcularMejorMovimientoBFS();

	void moverLocal(Direccion dir);
	std::shared_ptr<Personaje> heroe;

public:

	PerseguidorIA(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<Personaje> heroe);
	~PerseguidorIA();

	void mover() override;
	void perder() override;
	void ganar() override;

};