
#include "JuegoStalker.hpp"

// TODO: Replace for UI class
#include <iostream>

void inicializarPosiciones(Laberinto& laberinto,
                           std::shared_ptr<UI> ui,
                           uint32_t semillaInicial,
                           Posicion& posicionInicialHeroe,
                           Posicion& posicionInicialPerseguidor,
                           Llave* llaves,
                           Posicion& salida
) {

    posicionInicialHeroe = laberinto.obtenerCasillaLibre();
    ui->actualizarEntidad(posicionInicialHeroe.posicionX, posicionInicialHeroe.posicionY, TipoEntidad::HEROE);

    do {
        posicionInicialPerseguidor = laberinto.obtenerCasillaLibre();

    } while (posicionInicialPerseguidor == posicionInicialHeroe);
    ui->actualizarEntidad(posicionInicialPerseguidor.posicionX, posicionInicialPerseguidor.posicionY, TipoEntidad::PERSEGUIDOR);


    for (int i = 0; i < NUM_LLAVES; i++) {
        do {
            llaves[i].posicion = laberinto.obtenerCasillaLibre();
            llaves[i].recolectada = false;
        } while (llaves[i].posicion == posicionInicialPerseguidor);
    }
    for (int i = 0; i < NUM_LLAVES; i++) {
        ui->actualizarEntidad(llaves[i].posicion.posicionX, llaves[i].posicion.posicionY, TipoEntidad::LLAVE);
        laberinto.llaves[i].posicionX = llaves[i].posicion.posicionX;
        laberinto.llaves[i].posicionY = llaves[i].posicion.posicionY;
    }

    do {
        salida = laberinto.obtenerCasillaLibre();
    } while (salida == llaves[0].posicion ||
             salida == llaves[1].posicion ||
             salida == llaves[2].posicion);
    laberinto.salida = salida;
    ui->actualizarEntidad(salida.posicionX, salida.posicionY, TipoEntidad::SALIDA);

}

int main(int argc, char* argv[])
{

    std::shared_ptr<UI> ui;

#ifdef CURSES
    ui = std::make_shared<UINcurses>();
#elif QT
    //	ui = std::make_shared<UIQT>();
#else
    if (ui == nullptr) {
        std::cout << "Ninguna biblioteca de UI disponible" << std::endl;
        return -1;
    }
#endif

    Laberinto laberinto(ui);

    std::shared_ptr<Personaje> heroe;
    std::shared_ptr<Personaje> perseguidor;

    // Fuente de entropía
    std::random_device fuente;

    // Semilla inicial única
    uint32_t semillaInicial;

    Posicion posicionInicialHeroe;
    Posicion posicionInicialPerseguidor;
    Llave llaves[3];
    int llavesRecolectadas = 0;

    Posicion salida;
    Estado estadoJuego = Estado::JUGANDO;


    ui->desplegarTexto("Esperando entrada del usuario");
    //ui->desplegarTexto("1 - Jugador unico");
    //ui->desplegarTexto("2 - Multijugador (heroe)");
    //ui->desplegarTexto("3 - Multijugador (perseguidor)");
    //ui->desplegarTexto("4 - Salir");

    Tecla opcion;
    do {
         opcion = ui->leerTeclado();
         std::this_thread::sleep_for(std::chrono::milliseconds(10));

    } while (opcion == Tecla::NADA);

    clear();

    if (opcion == Tecla::UNO) {
        // Un solo jugador

        // Generar semilla inicial
        semillaInicial = fuente();
    
        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        laberinto.dibujar();

        inicializarPosiciones(laberinto, ui, semillaInicial, posicionInicialHeroe, posicionInicialPerseguidor,
            llaves, salida);

        // Local (teclado)
        heroe = std::make_shared<HeroeLocalSolo>(posicionInicialHeroe, ui, laberinto);

        // Perseguidor IA
        perseguidor = std::make_shared<PerseguidorIA>(posicionInicialPerseguidor, ui, laberinto, heroe);

    }
    else if (opcion == Tecla::DOS) {
        // Multijugador (heroe)

        // Generar semilla inicial
        semillaInicial = fuente();

        // Servidor de red para recibir al jugador remoto
        auto red = std::make_shared<ServidorRed>(ui);
        red->inicializar(sizeof(Posicion));

        // Mandar semilla inicial al jugador remoto
        red->enviarDatos_sync(&semillaInicial, sizeof(semillaInicial));

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        laberinto.dibujar();

        inicializarPosiciones(laberinto, ui, semillaInicial, posicionInicialHeroe, posicionInicialPerseguidor,
            llaves, salida);

        // Local (teclado) + red
        heroe = std::make_shared<HeroeLocalMulti>(posicionInicialHeroe, ui, laberinto, red);

        // Perseguidor humano remoto
        perseguidor = std::make_shared<PerseguidorHumanoRemoto>(posicionInicialPerseguidor, ui, laberinto, red);

    }
    else if (opcion == Tecla::TRES) {
        // Multijugador (perseguidor)

        // Cliente de red para conectarse a la partida
        auto red = std::make_shared<ClienteRed>(ui);
        red->inicializar(sizeof(Posicion));

        // Obtener la semilla inicial del server
        red->leerDatos_sync(&semillaInicial, sizeof(semillaInicial));

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        laberinto.dibujar();

        inicializarPosiciones(laberinto, ui, semillaInicial, posicionInicialHeroe, posicionInicialPerseguidor,
            llaves, salida);

        // Humano remoto (servidor)
        heroe = std::make_shared<HeroeRemoto>(posicionInicialHeroe, ui, laberinto, red);

        // Humano local (cliente)
        perseguidor = std::make_shared<PerseguidorHumanoLocal>(posicionInicialPerseguidor, ui, laberinto, red);

    }
    else if (opcion == Tecla::CUATRO) {

        return 0;
    }

    int contadorFrames = 0;

	// Game loop
    while (estadoJuego == Estado::JUGANDO) {
        // Marcar el inicio del frame
        auto inicioFrame = std::chrono::steady_clock::now();

        heroe->mover();

        if (contadorFrames % FRAMES_ESPERA_STALKER == 0) {
            perseguidor->mover();
        }

        // Lógica de Colisiones
        if (heroe->posicion == perseguidor->posicion) {
            estadoJuego = Estado::PERDIDO;
        }

        // Lógica de Llaves y Salida
        if (estadoJuego != Estado::PERDIDO) {
            for (int i = 0; i < NUM_LLAVES; i++) {
                if (heroe->posicion == llaves[i].posicion && llaves[i].recolectada != true) {
                    llaves[i].recolectada = true;
                    llavesRecolectadas++;
                }
            }

            bool todasLlaves = true;
            for (int i = 0; i < NUM_LLAVES; i++) {
                if (llaves[i].recolectada == false) {
                    todasLlaves = false;
                    break;
                }
            }

            if (todasLlaves) {
                if (heroe->posicion == salida) {
                    estadoJuego = Estado::GANADO;
                }
            }
            else {
                ui->desplegarTexto("Llaves restantes: " + std::to_string(NUM_LLAVES - llavesRecolectadas));
            }
        }

        // Dibujar en pantalla
        ui->render();

        // Aumentar el reloj interno
        contadorFrames++;

        // CÁLCULO DE FPS EXACTOS
        auto finFrame = std::chrono::steady_clock::now();
        auto tiempoTranscurrido = std::chrono::duration_cast<std::chrono::milliseconds>(finFrame - inicioFrame);

        // Si nos sobró tiempo, dormimos el hilo
        if (tiempoTranscurrido < duracionFrame) {
            std::this_thread::sleep_for(duracionFrame - tiempoTranscurrido);
        }
    }
    // Termina el juego

    if (estadoJuego == Estado::PERDIDO) {
        heroe->perder();
        perseguidor->ganar();

    }
    else if (estadoJuego == Estado::GANADO) {
        heroe->ganar();
        perseguidor->perder();
    }

    // Detener el juego
    do {
        opcion = ui->leerTeclado();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    } while (opcion == Tecla::NADA);

	return 0;
}
