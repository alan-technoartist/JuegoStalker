#include "ClienteRed.hpp"
#include <iostream>

#include <boost/asio/write.hpp>

using namespace boost::asio;

void ClienteRed::enviarDatos(void* data) {
    // Asignar buffer
    // TODO: verificar posibles condiciones de carrera

}

void ClienteRed::leerDatos(void* buf) {
    // Copiar datos del buffer 
    // TODO: verificar posibles condiciones de carrera
}

void ClienteRed::inicializar() {

    ip::tcp::resolver resolver(io_ctx);
    auto socket = std::make_shared<ip::tcp::socket>(io_ctx);

    // Conectar y recibir handshake inicial de forma síncrona
    boost::asio::connect(*socket, resolver.resolve("localhost", "5000"));
    //boost::asio::read(*socket, boost::asio::buffer(&init, sizeof(init)));

    // Lanzar bucle asíncrono
    co_spawn(io_ctx, loop(socket), detached);

    // Lanzar motor de asio en hilo separado
    io_thread = std::thread([&] {
        io_ctx.run();
    });
}

awaitable<void> ClienteRed::loop(std::shared_ptr<ip::tcp::socket> socket) {
    //while (true) 
    //  async_read
    //  async_write

}