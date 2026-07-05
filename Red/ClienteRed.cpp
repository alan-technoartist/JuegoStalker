#include "ClienteRed.hpp"

// TODO: usar clase UI
#include <iostream>

void ClienteRed::inicializar() {

    ip::tcp::resolver resolver(io_ctx);

    // Asignar socket
    socket = std::make_shared<ip::tcp::socket>(io_ctx);

    std::cout << "Conectando..." << std::endl;

    // Conectar al servidor
    boost::asio::connect(*socket, resolver.resolve("localhost", "6005"));

    std::cout << "Conectado al Servidor! "<< std::endl;

    // Lanzar motor de asio en un hilo separado
    io_thread = std::thread([&] {
        io_ctx.run();
    });
}

awaitable<void> ClienteRed::loop(std::shared_ptr<ip::tcp::socket> socket) {
    try {
        while (true) {
            co_await boost::asio::async_write(*socket, boost::asio::buffer(send_buf));
            std::cout << "Escrito al server: " << send_buf[0] << std::endl;
            co_await boost::asio::async_read(*socket, boost::asio::buffer(recv_buf));
            std::cout << "Leido del server: " << recv_buf[0] << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}