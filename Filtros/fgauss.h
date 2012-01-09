#ifndef FGAUSS_H
#define FGAUSS_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FGauss;
}

class FGauss : public QWidget
{
    Q_OBJECT

public:
    explicit FGauss(QWidget *parent = 0, Imagen image = Imagen());
    ~FGauss();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FGauss *ui;

    Imagen imagenOrig;

    double fG(double x); //funcion de Gauss unidimensional

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FGAUSS_H
