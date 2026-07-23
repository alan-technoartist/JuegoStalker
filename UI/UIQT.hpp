#pragma once

#include <QObject>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QMap>
#include <QPair>

#include "../Common/Common.hpp"
#include "UI.hpp"

constexpr int CELL_SIZE = 28;

class QtUI : public QObject, public UI {
    Q_OBJECT

public:
    QtUI(QObject* parent = nullptr);

    ~QtUI() override = default;

    Tecla leerTeclado() override;

    void desplegarTexto(std::string texto) override;

    void dibujarPared(int x, int y) override;

    void borrarCelda(int x, int y) override;

    void actualizarEntidad(int x, int y, TipoEntidad entidad) override;

    void render() override;

protected:
    // Capturar teclas del QGraphicsView — equivalente a getch()
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setCellColor(int x, int y, QColor color);

    QGraphicsScene* scene_;
    QGraphicsView* view_;
    QGraphicsTextItem* texto_status_;
    QMap<QPair<int, int>, QGraphicsItem*> celdas_;
    Tecla                                ultima_tecla_;
};