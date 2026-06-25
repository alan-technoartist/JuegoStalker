#include <boost/asio.hpp>

using namespace boost::asio;

class ClienteRed {
private:
	io_context     io_ctx;
	std::thread          io_thread;
	std::vector<uint8_t> send_buf;
	std::vector<uint8_t> recv_buf;

	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket);

public:
	// Interfaces para el personaje
	void enviarDatos(void* data);
	void leerDatos(void* buf);

	// Interfaz para main
	void inicializar();
};