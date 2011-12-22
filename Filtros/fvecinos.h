#ifndef FVECINOS_H
#define FVECINOS_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FVecinos;
}

class FVecinos : public QWidget
{
    Q_OBJECT

public:
    explicit FVecinos(QWidget *parent = 0, Imagen image = Imagen());
    ~FVecinos();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FVecinos *ui;

    Imagen imagenOriginal;

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FVECINOS_H
