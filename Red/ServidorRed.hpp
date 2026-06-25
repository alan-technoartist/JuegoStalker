#include <boost/asio.hpp>

using namespace boost::asio;

class ServidorRed {
private:
	io_context     io_ctx;
	std::thread          io_thread_;
	std::vector<uint8_t> send_buf_;
	std::vector<uint8_t> recv_buf_;

	awaitable<void> loop(std::shared_ptr<ip::tcp::socket> socket);

public:
	// Interfaces para el personaje
	void enviarDatos(void* data);
	void leerDatos(void* buf);

	// Interfaz para main
	void inicializar();
};