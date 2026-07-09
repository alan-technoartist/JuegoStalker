#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <mutex>

#include "../UI/UI.hpp"

using namespace boost::asio;

class RedBase {
protected:
	std::shared_ptr<UI> ui;
	io_context io_ctx;
	std::thread io_thread;
	std::shared_ptr<ip::tcp::socket> socket;

	std::mutex mutex_send_buf;
	std::vector<uint8_t> send_buf;

	std::mutex mutex_recv_buf;
	std::vector<uint8_t> recv_buf;


	// Implementaci�n depende del rol (cliente/servidor)
	virtual awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket) = 0;

public:
	RedBase(std::shared_ptr<UI> ui);
	virtual ~RedBase();

	// IO as�ncrono
	void enviarDatos(void* datos, int tamano);
	void leerDatos(void* datos, int tamano);

	// IO s�ncrono
	void enviarDatos_sync(const void* datos, int tamano);
	void leerDatos_sync(void* datos, int tamano);

	// Implementaci�n depende del rol (cliente/servidor)
	virtual void inicializar(size_t tamanoMensaje) = 0;

};