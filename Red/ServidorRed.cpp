#include "ServidorRed.hpp"
#include <iostream>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

void ServidorRed::enviarDatos(void* data) {
    // Asignar buffer
    // TODO: verificar posibles condiciones de carrera

}

void ServidorRed::leerDatos(void* buf) {
    // Copiar datos del buffer 
    // TODO: verificar posibles condiciones de carrera
}

void ServidorRed::inicializar() {
    // Crear acceptor
    ip::tcp::acceptor aceptador(io_ctx, ip::tcp::endpoint( ip::tcp::v4(), 5000 ));

    // Crear el socket
    auto socket = std::make_shared<ip::tcp::socket>(io_ctx);

    // Escuchar y aceptar conexiones
    aceptador.accept(*socket);

    // Enviar handshake inicial
    //boost::asio::write(*socket, boost::asio::buffer(&init, sizeof(init)));

    // Lanzar bucle asíncrono
    co_spawn(io_ctx, loop(socket), detached);

    // Lanzar motor de asio en un hilo separado
    io_thread_ = std::thread([&] {
        io_ctx.run();
    });
}

awaitable<void> ServidorRed::loop(std::shared_ptr<ip::tcp::socket> socket) {
    //while (true) 
    //  async_write
    //  async_read
}