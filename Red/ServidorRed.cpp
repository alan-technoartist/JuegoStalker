#include "ServidorRed.hpp"

// TODO: usar clase UI
#include <iostream>

void ServidorRed::inicializar() {
    // Crear acceptor
    ip::tcp::acceptor aceptador(io_ctx, ip::tcp::endpoint( ip::tcp::v4(), 6005 ));

    // Asignar el socket
    socket = std::make_shared<ip::tcp::socket>(io_ctx);

    std::cout << "Esperando clientes..." << std::endl;

    // Escuchar y aceptar conexiones
    aceptador.accept(*socket);

    std::cout << "Cliente conectado!" << std::endl;

    // Lanzar motor de asio en un hilo separado
    io_thread = std::thread([&] {
        io_ctx.run();
    });
}

awaitable<void> ServidorRed::loop(std::shared_ptr<ip::tcp::socket> socket) {
    try {
        while (true) {
            co_await boost::asio::async_read(*socket, boost::asio::buffer(recv_buf));
            std::cout << "Leido del cliente: " << recv_buf[0] << std::endl;
            co_await boost::asio::async_write(*socket, boost::asio::buffer(send_buf));
            std::cout << "Escrito al cliente: " << send_buf[0] << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}