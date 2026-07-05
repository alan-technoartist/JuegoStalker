#include "RedBase.hpp"

// TODO: usar clase UI
#include <iostream>

RedBase::~RedBase() {
    // TODO: Rutinas de limpieza
    io_thread.join();
    io_ctx.stop();
}

void RedBase::enviarDatos_sync(const void* datos, int tamano) {

    boost::asio::write(*socket, boost::asio::buffer(datos, tamano));
}

void RedBase::leerDatos_sync(void* datos, int tamano) {
    boost::asio::read(*socket, boost::asio::buffer(datos, tamano));
}

void RedBase::enviarDatos(void* datos, int tamano) {
    // Asignar buffer
    // TODO: verificar posibles condiciones de carrera
    send_buf.assign(static_cast<const uint8_t*>(datos),
                    static_cast<const uint8_t*>(datos) + tamano);
}

void RedBase::leerDatos(void* datos, int tamano) {
    // Copiar datos del buffer 
    // TODO: verificar posibles condiciones de carrera
    if (recv_buf.size() < tamano) {
        std::cerr << "Se recibió menos del tamano del esperado" << std::endl;
        return;
    }
    std::memcpy(datos, recv_buf.data(), tamano);

    // Limpiar el buffer temporal
    recv_buf.clear();
}

void RedBase::iniciarIOAsincrono(int tamanoMensaje) {
    // Iniciar tamaño de los búferes
    recv_buf.resize(tamanoMensaje);
    send_buf.resize(tamanoMensaje);

    // Lanzar bucle asíncrono
    co_spawn(io_ctx, loop(socket), detached);


}
