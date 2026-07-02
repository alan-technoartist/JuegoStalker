#pragma once

#include <boost/asio.hpp>
#include "Red.hpp"

using namespace boost::asio;

class ServidorRed {
private:
	io_context io_ctx;
	std::thread io_thread;

	std::vector<uint8_t> send_buf;
	std::vector<uint8_t> recv_buf;

	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket);

public:
	void enviarDatos(void* datos, int tamano);

	void leerDatos(void* datos, int tamano);

	void inicializar(int tamanoMensaje);
};