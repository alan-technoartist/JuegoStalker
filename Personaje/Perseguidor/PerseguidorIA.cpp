#include "PerseguidorIA.hpp"

PerseguidorIA::PerseguidorIA(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, Posicion posicionHeroe) :
 Personaje(posicionInicial, ui, laberinto) {
	this->posicionHeroe = posicionHeroe;

	// 1. Inicializar ONNX una sola vez
	env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "ModeloStalker");
	Ort::SessionOptions session_options;

	std::string archivo_onnx = "stalker.onnx";
	std::wstring model_path_w(archivo_onnx.begin(), archivo_onnx.end());

	session = std::make_unique<Ort::Session>(*env, model_path_w.c_str(), session_options);
	memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
}

PerseguidorIA::~PerseguidorIA() {

}

void PerseguidorIA::construirEntrada(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 >& datosEntrada) {
	// Construír el tensor de entrada
	// - Laberinto (Si el laberinto es 21x21 => 441)
	// - Posición Heroe (x, y => 2)
	// - Posición Perseguidor (x, y => 2)

	for (int i = 0; i < LAB_HEIGHT; i++) {
		for (int j = 0; j < LAB_WIDTH; j++) {
			int indice = i * LAB_HEIGHT + j;

            // "Aplanar" laberinto
			datosEntrada[indice] = (laberinto.esPared(i, j)) ? 1.0f : 0.0f;
		}
	}

	// Posición perseguidor
	datosEntrada[441] = posicion.posicionX;
	datosEntrada[442] = posicion.posicionY;

    // Posición heroe
	datosEntrada[443] = posicionHeroe.posicionX;
	datosEntrada[444] = posicionHeroe.posicionY;

}

Direccion PerseguidorIA::ejecutarInferencia(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 > datosEntrada) {
	Ort::AllocatorWithDefaultOptions allocator;
	Ort::AllocatedStringPtr input_name_ptr = session->GetInputNameAllocated(0, allocator);
	Ort::AllocatedStringPtr output_name_ptr = session->GetOutputNameAllocated(0, allocator);
	const char* input_names[] = { input_name_ptr.get() };
	const char* output_names[] = { output_name_ptr.get() };

	std::vector<int64_t> input_shape = { 1, LAB_HEIGHT * LAB_WIDTH + 4 };

	// Usamos el memory_info de la clase
	Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
		memory_info, datosEntrada.data(), datosEntrada.size(), input_shape.data(), input_shape.size());

	// Inferencia ligera
	auto output_tensors = session->Run(
		Ort::RunOptions{ nullptr }, input_names, &input_tensor, 1, output_names, 1);

	float* output_data = output_tensors.front().GetTensorMutableData<float>();

	int direccionInferida = 0;
	float prob_maxima = output_data[0];
	for (int i = 1; i < 4; ++i) {
		if (output_data[i] > prob_maxima) {
			prob_maxima = output_data[i];
			direccionInferida = i;
		}
	}

	return static_cast<Direccion>(direccionInferida);
}


void PerseguidorIA::moverLocal(Direccion dir) {
	switch (dir) {
	case Direccion::IZQUIERDA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY - 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionY -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::DERECHA:
		if (!(laberinto.esPared(posicion.posicionX, posicion.posicionY + 1))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionY += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::ARRIBA:
		if (!(laberinto.esPared(posicion.posicionX - 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionX -= 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	case Direccion::ABAJO:
		if (!(laberinto.esPared(posicion.posicionX + 1, posicion.posicionY))) {

			if (!laberinto.esSalida(posicion.posicionX, posicion.posicionY) &&
				!laberinto.hayLlave(posicion.posicionX, posicion.posicionY))
				ui->borrarCelda(posicion.posicionX, posicion.posicionY);
			else {
				if (laberinto.esSalida(posicion.posicionX, posicion.posicionY))
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::SALIDA);
				else
					ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::LLAVE);
			}

			posicion.posicionX += 1;
			ui->actualizarEntidad(posicion.posicionX, posicion.posicionY, TipoEntidad::PERSEGUIDOR);
		}
		break;
	}
}



void PerseguidorIA::mover() {
	// Laberinto + posiciones
	std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 > datosEntrada;

	// Construír tensor de entrada
	construirEntrada(datosEntrada);

	// Ejecutar inferencia en hilo separado
	auto futuro = std::async(std::launch::async, [this, datosEntrada] {
		return ejecutarInferencia(datosEntrada);
		});

	// Obtener salida
    Direccion direccion = futuro.get();

	moverLocal(direccion);
}

void PerseguidorIA::perder() {

}

void PerseguidorIA::ganar() {

}