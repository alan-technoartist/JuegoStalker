#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <thread>

using namespace boost::asio;

class RedBase {
protected:
	io_context io_ctx;
	std::thread io_thread;
	std::shared_ptr<ip::tcp::socket> socket;
	std::vector<uint8_t> send_buf;
	std::vector<uint8_t> recv_buf;

	// Implementación depende del rol (cliente/servidor)
	virtual awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket) = 0;

public:
	virtual ~RedBase();

	// IO asíncrono
	virtual void enviarDatos(void* datos, int tamano);
	virtual void leerDatos(void* datos, int tamano);

	// IO síncrono
	virtual void enviarDatos_sync(const void* datos, int tamano);
	virtual void leerDatos_sync(void* datos, int tamano);

	// Iniciar loop asíncrono
	void iniciarIOAsincrono(int tamanoMensaje);

	// Implementación depende del rol (cliente/servidor)
	virtual void inicializar() = 0;

};