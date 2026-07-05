
#include "JuegoStalker.hpp"

// TODO: Replace for UI class
#include <iostream>

void inicializarPosiciones(Laberinto& laberinto,
                           uint32_t semillaInicial,
                           Posicion& posicionInicialHeroe,
                           Posicion& posicionInicialPerseguidor,
                           Llave* llaves,
                           Posicion& salida
) {

    posicionInicialHeroe = laberinto.obtenerCasillaLibre();

    do {
        posicionInicialPerseguidor = laberinto.obtenerCasillaLibre();
    } while (posicionInicialPerseguidor == posicionInicialHeroe);

    for (int i = 0; i < NUM_LLAVES; i++) {
        llaves[i].posicion = laberinto.obtenerCasillaLibre();
        llaves[i].recolectada = false;
    }

    salida = laberinto.obtenerCasillaLibre();

}

int main(int argc, char* argv[])
{

    std::shared_ptr<UI> ui;

#ifdef CURSES
    //ui = std::make_shared<UINcurses>();
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
    Posicion salida;

    int opcion;
    std::cout << "Selecciona opcion" << std::endl;
    std::cout << "1 - Crear partida nueva (solo)" << std::endl;
    std::cout << "2 - Crear partida nueva (multijugador)" << std::endl;
    std::cout << "3 - Unirse a una partida como perseguidor" << std::endl;
    std::cout << "4 - Salir" << std::endl;

    std::cin >> opcion;

    if (opcion == 1) {
        // Un solo jugador
    
        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        // Local (teclado)
        heroe = std::make_shared<HeroeLocal>(posicionInicialHeroe);

        // Perseguidor IA
        perseguidor = std::make_shared<PerseguidorIA>();

    }
    else if (opcion == 2) {
        // Multijugador (heroe)

        // Generar semilla inicial
        semillaInicial = fuente();

        // Servidor de red para recibir al jugador remoto
        auto red = std::make_shared<ServidorRed>();
        red->inicializar();

        std::cout << "Enviando semilla inicial: " << semillaInicial << std::endl;

        // Mandar semilla inicial al jugador remoto
        red->enviarDatos_sync(&semillaInicial, sizeof(semillaInicial));

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        inicializarPosiciones(laberinto, semillaInicial, posicionInicialHeroe, posicionInicialPerseguidor,
            llaves, salida);

        // Local (teclado)
        heroe = std::make_shared<HeroeLocal>(posicionInicialHeroe);

        // Perseguidor humano remoto
        perseguidor = std::make_shared<PerseguidorHumanoRemoto>(posicionInicialPerseguidor, red);

        // Iniciar loop asíncrono para enviar/recibir posiciones
        red->iniciarIOAsincrono(sizeof(posicionInicialPerseguidor));

    }
    else if (opcion == 3) {
        // Multijugador (perseguidor)

        // Cliente de red para conectarse a la partida
        auto red = std::make_shared<ClienteRed>();
        red->inicializar();

        // Obtener la semilla inicial del server
        red->leerDatos_sync(&semillaInicial, sizeof(semillaInicial));

        std::cout << "Semilla inicial recibida: " << semillaInicial << std::endl;

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();

        inicializarPosiciones(laberinto, semillaInicial, posicionInicialHeroe, posicionInicialPerseguidor,
            llaves, salida);

        // Humano remoto (servidor)
        heroe = std::make_shared<HeroeRemoto>(posicionInicialHeroe);

        // Humano local (cliente)
        perseguidor = std::make_shared<PerseguidorHumanoLocal>(posicionInicialPerseguidor, red);

        // Iniciar loop asíncrono para enviar/recibir posiciones
        red->iniciarIOAsincrono(sizeof(posicionInicialPerseguidor));
    }

    bool gameRunning = true;

	// Game loop
	while(gameRunning) {
        //heroe->mover();
        //perseguidor->mover();

	}

	return 0;
}
