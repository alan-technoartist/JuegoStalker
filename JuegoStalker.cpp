#include "JuegoStalker.hpp"
#include <iostream>

#ifdef CURSES
#include "UI/UINcurses.hpp"
#elif defined(QT)
#include "UI/UIQT.hpp"
#include <QApplication>
#include <QTimer>
#endif

void inicializarPosiciones(Laberinto& laberinto,
    std::shared_ptr<UI> ui,
    uint32_t semillaInicial,
    Posicion& posicionInicialHeroe,
    Posicion& posicionInicialPerseguidor,
    Llave* llaves,
    Posicion& salida)
{
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


// =====================================================================
// Estado global del juego — necesario para el QTimer callback
// =====================================================================
struct EstadoPartida {
    std::shared_ptr<UI>        ui;
    std::shared_ptr<Personaje> heroe;
    std::shared_ptr<Personaje> perseguidor;
    Llave                      llaves[NUM_LLAVES];
    Posicion                   salida;
    Estado                     estadoJuego = Estado::JUGANDO;
    int                        contadorFrames = 0;
    int                        llavesRecolectadas = 0;
};

// Un frame del game loop — mismo código en Qt y ncurses
bool ejecutarFrame(EstadoPartida& p)
{
    p.heroe->mover();

    if (p.contadorFrames % FRAMES_ESPERA_STALKER == 0)
        p.perseguidor->mover();

    // Colisiones
    if (p.heroe->posicion == p.perseguidor->posicion)
        p.estadoJuego = Estado::PERDIDO;

    // Llaves y salida
    if (p.estadoJuego != Estado::PERDIDO) {
        for (int i = 0; i < NUM_LLAVES; i++) {
            if (p.heroe->posicion == p.llaves[i].posicion && !p.llaves[i].recolectada) {
                p.llaves[i].recolectada = true;
                p.llavesRecolectadas++;
            }
        }

        bool todasLlaves = true;
        for (int i = 0; i < NUM_LLAVES; i++) {
            if (!p.llaves[i].recolectada) { todasLlaves = false; break; }
        }

        if (todasLlaves) {
            if (p.heroe->posicion == p.salida)
                p.estadoJuego = Estado::GANADO;
        }
        else {
            p.ui->desplegarTexto("Llaves restantes: " +
                std::to_string(NUM_LLAVES - p.llavesRecolectadas));
        }
    }

    p.ui->render();
    p.contadorFrames++;

    return p.estadoJuego == Estado::JUGANDO;
}

void terminarJuego(EstadoPartida& p)
{
    if (p.estadoJuego == Estado::PERDIDO) {
        p.heroe->perder();
        p.perseguidor->ganar();
    }
    else if (p.estadoJuego == Estado::GANADO) {
        p.heroe->ganar();
        p.perseguidor->perder();
    }
}


// =====================================================================
// Main
// =====================================================================
int main(int argc, char* argv[])
{
    // -----------------------------------------------------------------
    // 1. Crear UI según el backend compilado
    // -----------------------------------------------------------------
    std::shared_ptr<UI> ui;

#ifdef CURSES
    ui = std::make_shared<UINcurses>();
#elif defined(QT)
    QApplication app(argc, argv);
    ui = std::make_shared<QtUI>();
#else
    std::cout << "Ninguna biblioteca de UI disponible" << std::endl;
    return -1;
#endif

    // -----------------------------------------------------------------
    // 2. Menú inicial — igual en ambos backends
    // -----------------------------------------------------------------
    ui->desplegarTexto("1-Solo  2-Multi(heroe)  3-Multi(perseguidor)  4-Salir");

    Tecla opcion;
    do {
        opcion = ui->leerTeclado();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
#ifdef QT
        // Qt necesita procesar eventos mientras espera input
        QApplication::processEvents();
#endif
    } while (opcion == Tecla::NADA);

#ifdef CURSES
    clear();  // solo ncurses
#endif

    if (opcion == Tecla::CUATRO)
        return 0;

    // -----------------------------------------------------------------
    // 3. Inicializar partida
    // -----------------------------------------------------------------
    auto p = std::make_shared<EstadoPartida>();
    p->ui = ui;

    std::random_device fuente;
    uint32_t semillaInicial;
    Laberinto laberinto(ui);

    if (opcion == Tecla::UNO) {
        semillaInicial = fuente();
        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();
        laberinto.dibujar();

        Posicion ph, pp;
        inicializarPosiciones(laberinto, ui, semillaInicial, ph, pp, p->llaves, p->salida);

        p->heroe = std::make_shared<HeroeLocalSolo>(ph, ui, laberinto);
        p->perseguidor = std::make_shared<PerseguidorIA>(pp, ui, laberinto, p->heroe);
    }
    else if (opcion == Tecla::DOS) {
        semillaInicial = fuente();
        auto red = std::make_shared<ServidorRed>(ui);
        red->inicializar(sizeof(Posicion));
        red->enviarDatos_sync(&semillaInicial, sizeof(semillaInicial));

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();
        laberinto.dibujar();

        Posicion ph, pp;
        inicializarPosiciones(laberinto, ui, semillaInicial, ph, pp, p->llaves, p->salida);

        p->heroe = std::make_shared<HeroeLocalMulti>(ph, ui, laberinto, red);
        p->perseguidor = std::make_shared<PerseguidorHumanoRemoto>(pp, ui, laberinto, red);
    }
    else if (opcion == Tecla::TRES) {
        auto red = std::make_shared<ClienteRed>(ui);
        red->inicializar(sizeof(Posicion));
        red->leerDatos_sync(&semillaInicial, sizeof(semillaInicial));

        laberinto.iniciarMotor(semillaInicial);
        laberinto.generar();
        laberinto.dibujar();

        Posicion ph, pp;
        inicializarPosiciones(laberinto, ui, semillaInicial, ph, pp, p->llaves, p->salida);

        p->heroe = std::make_shared<HeroeRemoto>(ph, ui, laberinto, red);
        p->perseguidor = std::make_shared<PerseguidorHumanoLocal>(pp, ui, laberinto, red);
    }

    // -----------------------------------------------------------------
    // 4. Game loop
    // -----------------------------------------------------------------
#ifdef CURSES
    // ncurses: while bloqueante con sleep manual
    while (p->estadoJuego == Estado::JUGANDO) {
        auto inicioFrame = std::chrono::steady_clock::now();

        ejecutarFrame(*p);

        auto fin = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicioFrame);
        if (elapsed < duracionFrame)
            std::this_thread::sleep_for(duracionFrame - elapsed);
    }
    terminarJuego(*p);

    // Esperar tecla final
    Tecla t;
    do {
        t = ui->leerTeclado();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } while (t == Tecla::NADA);

#elif defined(QT)
    // Qt: QTimer dispara cada frame — no bloquea el event loop
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        auto inicioFrame = std::chrono::steady_clock::now();

        bool jugando = ejecutarFrame(*p);

        if (!jugando) {
            timer.stop();
            terminarJuego(*p);
            // Esperar tecla final antes de cerrar
            QTimer::singleShot(0, [&]() {
                Tecla t = Tecla::NADA;
                while (t == Tecla::NADA) {
                    t = ui->leerTeclado();
                    QApplication::processEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                QApplication::quit();
                });
        }

        auto fin = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicioFrame);
        if (elapsed < duracionFrame)
            std::this_thread::sleep_for(duracionFrame - elapsed);
        });

    timer.start(std::chrono::duration_cast<std::chrono::milliseconds>(duracionFrame).count());
    return app.exec();
#endif

    return 0;
}