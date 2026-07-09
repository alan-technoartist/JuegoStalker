#include "ClienteRed.hpp"

ClienteRed::ClienteRed(std::shared_ptr<UI> ui) :
    RedBase(ui) {
}

void ClienteRed::inicializar(size_t tamanoMensaje) {

    ip::tcp::resolver resolver(io_ctx);

    // Asignar socket
    socket = std::make_shared<ip::tcp::socket>(io_ctx);

    //ui->desplegarTexto("Conectando al servidor...");

    // Conectar al servidor
    boost::asio::connect(*socket, resolver.resolve("localhost", "6005"));

    //ui->desplegarTexto("Conectado!");

    tamanoMensaje = 2;
    // Iniciar tama�o de los b�feres
    recv_buf.resize(tamanoMensaje);
    send_buf.resize(tamanoMensaje);

    // Registrar trabajo as�ncrono
    co_spawn(io_ctx, loop(socket), detached);

    // Lanzar motor de asio en un hilo separado
    io_thread = std::thread([this] {
        io_ctx.run();
    });
}

awaitable<void> ClienteRed::loop(std::shared_ptr<ip::tcp::socket> socket) {
    //ui->desplegarTexto("Iniciando loop de red...");

    try {
        while (true) {
            {
                std::lock_guard<std::mutex> guard(mutex_send_buf); // mutex.lock()

                co_await boost::asio::async_write(*socket, boost::asio::buffer(send_buf), use_awaitable);
                //ui->desplegarTexto("Escrito");
            }

            {
                std::lock_guard<std::mutex> guard(mutex_recv_buf); // mutex.lock()

                co_await boost::asio::async_read(*socket, boost::asio::buffer(recv_buf), use_awaitable);
                //ui->desplegarTexto("Leido");
            }

        }
    }
    catch (std::exception& e) {
        ui->desplegarTexto(e.what());

    }
}