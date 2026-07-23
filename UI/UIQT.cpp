#include "UIQT.hpp"

QtUI::QtUI(QObject* parent) : QObject(parent)
{
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_);

    view_->setWindowTitle("Grid Stalker");
    view_->setBackgroundBrush(QColor(20, 20, 30));
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->installEventFilter(this);
    view_->show();

    ultima_tecla_ = Tecla::NADA;

    // Texto de status — equivalente a mvprintw(21, 0, ...)
    texto_status_ = scene_->addText("");
    texto_status_->setDefaultTextColor(Qt::white);
    texto_status_->setPos(0, LAB_HEIGHT * CELL_SIZE + 4);
}


// ------------------------------------------------------------------
// leerTeclado — equivalente a getch() con nodelay
// No bloqueante: devuelve NADA si no hay tecla
// ------------------------------------------------------------------
Tecla QtUI::leerTeclado()
{
    Tecla t = ultima_tecla_;
    ultima_tecla_ = Tecla::NADA;
    return t;
}

// ------------------------------------------------------------------
// desplegarTexto — equivalente a mvprintw(21, 0, texto)
// ------------------------------------------------------------------
void QtUI::desplegarTexto(std::string texto) 
{
    texto_status_->setPlainText(QString::fromStdString(texto));
}

// ------------------------------------------------------------------
// dibujarPared — equivalente a mvaddch(x, y, ACS_BLOCK)
// ------------------------------------------------------------------
void QtUI::dibujarPared(int x, int y) 
{
    setCellColor(x, y, QColor(50, 50, 80));
}

// ------------------------------------------------------------------
// borrarCelda — equivalente a mvaddch(x, y, ' ')
// ------------------------------------------------------------------
void QtUI::borrarCelda(int x, int y) 
{
    setCellColor(x, y, QColor(20, 20, 30));
}

// ------------------------------------------------------------------
// actualizarEntidad — equivalente a mvaddch(x, y, char)
// ------------------------------------------------------------------
void QtUI::actualizarEntidad(int x, int y, TipoEntidad entidad) 
{
    switch (entidad) {
    case TipoEntidad::HEROE:
        setCellColor(x, y, QColor(80, 200, 120));   // verde — 'X'
        break;
    case TipoEntidad::PERSEGUIDOR:
        setCellColor(x, y, QColor(220, 80, 80));    // rojo  — '@'
        break;
    case TipoEntidad::LLAVE:
        setCellColor(x, y, QColor(240, 200, 60));   // amarillo — '*'
        break;
    case TipoEntidad::SALIDA:
        setCellColor(x, y, QColor(120, 80, 220));   // morado — 'E'
        break;
    }
}

// ------------------------------------------------------------------
// render — equivalente a refresh()
// ------------------------------------------------------------------
void QtUI::render() 
{
    QApplication::processEvents();
}


// Capturar teclas del QGraphicsView — equivalente a getch()
bool QtUI::eventFilter(QObject* obj, QEvent* event) 
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        switch (key->key()) {
        case Qt::Key_Up:     ultima_tecla_ = Tecla::ARRIBA;    break;
        case Qt::Key_Down:   ultima_tecla_ = Tecla::ABAJO;     break;
        case Qt::Key_Left:   ultima_tecla_ = Tecla::IZQUIERDA; break;
        case Qt::Key_Right:  ultima_tecla_ = Tecla::DERECHA;   break;
        case Qt::Key_1:      ultima_tecla_ = Tecla::UNO;       break;
        case Qt::Key_2:      ultima_tecla_ = Tecla::DOS;       break;
        case Qt::Key_3:      ultima_tecla_ = Tecla::TRES;      break;
        case Qt::Key_4:      ultima_tecla_ = Tecla::CUATRO;    break;
        default: break;
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}


void QtUI::setCellColor(int x, int y, QColor color)
{
    auto key = QPair<int, int>(x, y);

    if (!celdas_.contains(key)) {
        auto* rect = scene_->addRect(
            y * CELL_SIZE,
            x * CELL_SIZE,
            CELL_SIZE - 1,
            CELL_SIZE - 1,
            QPen(Qt::NoPen),
            QBrush(color)
        );
        celdas_[key] = rect;
    }
    else {
        auto* rect = static_cast<QGraphicsRectItem*>(celdas_[key]);
        rect->setBrush(QBrush(color));
    }
}
