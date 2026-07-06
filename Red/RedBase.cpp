#include "RedBase.hpp"

RedBase::RedBase(std::shared_ptr<UI> ui) {
    this->ui = ui;
}

RedBase::~RedBase() {
    // TODO: Rutinas de limpieza
    io_ctx.stop();
    io_thread.join();
}

void RedBase::enviarDatos_sync(const void* datos, int tamano) {

    boost::asio::write(*socket, boost::asio::buffer(datos, tamano));
}

void RedBase::leerDatos_sync(void* datos, int tamano) {
    boost::asio::read(*socket, boost::asio::buffer(datos, tamano));
}

std::string imprimirDatos(const uint8_t* datos, int tamano) {
    std::ostringstream oss;

    for (int i = 0; i < tamano; ++i) {
        oss << std::setw(2) << std::setfill('0') << static_cast<int>(datos[i]) << " ";
    }

    return oss.str();
}


void RedBase::enviarDatos(void* datos, int tamano) {
    // Asignar buffer
    // TODO: verificar posibles condiciones de carrera
    if (tamano > send_buf.size()) {
        ui->desplegarTexto("Error al recibir datos");
        return;
    }

    std::memcpy(send_buf.data(), datos, tamano);

    std::string mensaje = "Enviando: " + imprimirDatos(send_buf.data(), tamano);
    ui->desplegarTexto(mensaje);
}

void RedBase::leerDatos(void* datos, int tamano) {

    // Copiar datos del buffer 
    // TODO: verificar posibles condiciones de carrera
   if (recv_buf.size() < tamano) {
        ui->desplegarTexto("Error al recibir datos");
        return;
    }
    std::memcpy(datos, recv_buf.data(), tamano);

    std::string mensaje = "Recibido: " + imprimirDatos(recv_buf.data(), tamano);
    ui->desplegarTexto(mensaje);
}
