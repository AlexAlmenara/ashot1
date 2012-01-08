#ifndef FDEF_H
#define FDEF_H

#include <QWidget>
#include <QtGui>
#include "imagen.h"

namespace Ui {
    class FDef;
}

class FDef : public QWidget
{
    Q_OBJECT

public:
    explicit FDef(QWidget *parent = 0, Imagen image = Imagen());
    ~FDef();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FDef *ui;

    Imagen imagenOrig; //imagen de partida
    QSpinBox ** ventana; //kernel del filtro

    QHBoxLayout * boxFilas; //horizontal layout para cada fila de la ventana
    QVBoxLayout boxVentana; //layout para toda la ventana

    bool firstPainted;
    int N;
    void quitarVentana();
    void initVentana();

public slots:
    void crearVentana();

    void cambiarImagen(); //aplicar
    void aceptar();
    void cancelar();

    void abrir(); //abre filtro desde fichero
    void guardar(); //guarda filtro actual a fichero


signals:
    void changed();
    void acepted();
};

#endif // FDEF_H
