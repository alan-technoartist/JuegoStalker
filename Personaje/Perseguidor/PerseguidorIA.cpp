#include "PerseguidorIA.hpp"

PerseguidorIA::PerseguidorIA(Posicion posicionInicial, std::shared_ptr<UI> ui, Laberinto& laberinto, std::shared_ptr<Personaje> heroe) :
 Personaje(posicionInicial, ui, laberinto) {
	this->heroe = heroe;

#ifdef ML
	// Inicializar ONNX una sola vez
	env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "ModeloStalker");
	Ort::SessionOptions session_options;

	std::string archivo_onnx = "stalker.onnx";
	std::wstring model_path_w(archivo_onnx.begin(), archivo_onnx.end());

	session = std::make_unique<Ort::Session>(*env, model_path_w.c_str(), session_options);
	memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
#endif
}

PerseguidorIA::~PerseguidorIA() {

}

#ifdef ML
void PerseguidorIA::construirEntrada(std::array< float, LAB_HEIGHT* LAB_WIDTH + 4 >& datosEntrada) {
	// Construír el tensor de entrada
	// - Laberinto (Si el laberinto es 21x21 => 441)
	// - Posición Heroe (x, y => 2)
	// - Posición Perseguidor (x, y => 2)

	for (int i = 0; i < LAB_HEIGHT; i++) {
		for (int j = 0; j < LAB_WIDTH; j++) {
			int indice = i * LAB_WIDTH + j;

			// "Aplanar" laberinto
			datosEntrada[indice] = (laberinto.esPared(i, j)) ? 1.0f : 0.0f;
		}
	}

	// Posición perseguidor (normalizada)
	datosEntrada[441] = static_cast<float>(posicion.posicionY) / LAB_HEIGHT;
	datosEntrada[442] = static_cast<float>(posicion.posicionX) / LAB_WIDTH;

	// Posición heroe (normalizada)
	datosEntrada[443] = static_cast<float>(heroe->posicion.posicionY) / LAB_HEIGHT;
	datosEntrada[444] = static_cast<float>(heroe->posicion.posicionX) / LAB_WIDTH;

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
#endif

Direccion PerseguidorIA::calcularMejorMovimientoBFS() {
	int startX = posicion.posicionX;
	int startY = posicion.posicionY;
	int targetX = heroe->posicion.posicionX;
	int targetY = heroe->posicion.posicionY;

	// Si ya estamos encima del héroe, no hacemos nada
	if (startX == targetX && startY == targetY) return Direccion::ARRIBA;

	// Matrices para el algoritmo
	std::vector<std::vector<bool>> visitado(LAB_HEIGHT, std::vector<bool>(LAB_WIDTH, false));

	// Guardará de qué casilla venimos para poder reconstruir el camino
	std::vector<std::vector<std::pair<int, int>>> padre(LAB_HEIGHT, std::vector<std::pair<int, int>>(LAB_WIDTH, { -1, -1 }));

	std::queue<std::pair<int, int>> cola;
	cola.push({ startX, startY });
	visitado[startX][startY] = true;

	bool heroeEncontrado = false;

	// Vectores de dirección: ARRIBA, ABAJO, IZQUIERDA, DERECHA
	// (Alineados con tu lógica de moverLocal: X es fila, Y es columna)
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	while (!cola.empty()) {
		auto [cx, cy] = cola.front();
		cola.pop();

		if (cx == targetX && cy == targetY) {
			heroeEncontrado = true;
			break;
		}

		// Explorar las 4 direcciones
		for (int i = 0; i < 4; i++) {
			int nx = cx + dx[i];
			int ny = cy + dy[i];

			// Validar límites y muros
			if (nx >= 0 && nx < LAB_HEIGHT && ny >= 0 && ny < LAB_WIDTH) {
				if (!visitado[nx][ny] && !laberinto.esPared(nx, ny)) {
					visitado[nx][ny] = true;
					padre[nx][ny] = { cx, cy };
					cola.push({ nx, ny });
				}
			}
		}
	}

	// Si el héroe está encerrado en muros (no hay ruta), nos quedamos quietos
	if (!heroeEncontrado) return Direccion::ARRIBA;

	// =================================================================
	// RECONSTRUIR EL CAMINO (De reversa desde el héroe hasta el stalker)
	// =================================================================
	int currX = targetX;
	int currY = targetY;

	// Retrocedemos hasta encontrar la casilla justo al lado del Stalker
	while (padre[currX][currY].first != startX || padre[currX][currY].second != startY) {
		auto p = padre[currX][currY];
		currX = p.first;
		currY = p.second;
	}

	// Traducir esa primera casilla a un comando de Direccion
	if (currX == startX - 1) return Direccion::ARRIBA;
	if (currX == startX + 1) return Direccion::ABAJO;
	if (currY == startY - 1) return Direccion::IZQUIERDA;
	if (currY == startY + 1) return Direccion::DERECHA;

	return Direccion::ARRIBA; // Fallback de seguridad
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

#ifdef ML
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
#else

	// Fallback: calculamos el movimiento exacto instantáneamente
	Direccion direccion = calcularMejorMovimientoBFS();

#endif

	// Ejecutamos (Tu método moverLocal se queda intacto)
	moverLocal(direccion);
}

void PerseguidorIA::perder() {

}

void PerseguidorIA::ganar() {

}