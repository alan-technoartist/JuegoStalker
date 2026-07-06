#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <thread>

#include "../UI/UI.hpp"

using namespace boost::asio;

class RedBase {
protected:
	std::shared_ptr<UI> ui;
	io_context io_ctx;
	std::thread io_thread;
	std::shared_ptr<ip::tcp::socket> socket;
	std::vector<uint8_t> send_buf;
	std::vector<uint8_t> recv_buf;

	// Implementación depende del rol (cliente/servidor)
	virtual awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket) = 0;

public:
	RedBase(std::shared_ptr<UI> ui);
	virtual ~RedBase();

	// IO asíncrono
	void enviarDatos(void* datos, int tamano);
	void leerDatos(void* datos, int tamano);

	// IO síncrono
	void enviarDatos_sync(const void* datos, int tamano);
	void leerDatos_sync(void* datos, int tamano);

	// Implementación depende del rol (cliente/servidor)
	virtual void inicializar(size_t tamanoMensaje) = 0;

};