#ifndef DIGITALIZAR_H
#define DIGITALIZAR_H

#include <QWidget>
#include "imagen.h"
#include "math.h"

namespace Ui {
    class Digitalizar;
}

class Digitalizar : public QWidget
{
    Q_OBJECT

public:
    explicit Digitalizar(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Digitalizar();

    Imagen imagenAux;

private:
    Ui::Digitalizar *ui;

    Imagen imagenOriginal;

    int width, height; //nuevas dimensiones de la imagen (muestreo)
    int nBits; //numero de bits simulados (cuantizacion)


public slots:
    void aceptar();
    void cancelar();


signals:
    void changed();

};

#endif // DIGITALIZAR_H
