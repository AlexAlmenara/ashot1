#ifndef HESPECIF_H
#define HESPECIF_H

#include <QWidget>
#include "imagen.h"
#include "Imagen/histograma.h"
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
    class Hespecif;
}

class Hespecif : public QWidget
{
    Q_OBJECT

public:
    explicit Hespecif(QWidget *parent = 0, Imagen image = Imagen());
    ~Hespecif();

    Imagen imagenAux; //imagen de trabajo a cambiar

private:
    Ui::Hespecif *ui;

    Imagen imagenOriginal;
    Imagen imagenEsp; //imagen elegida para copiar su histograma a imagen
    QLabel label; //label donde se vera la nueva imagen. atencion saldria sencillo, no en un aShot nuevo

    double distancia(int vin0, int vin1); //niveles: vin0 de imagen, vin1 de imagenEsp
    int minDist(int vin0); //devuelve nivel vin1 de imagenEsp, tal que sea minima distncia de vin0 de imagen

    void updateLabel();

public slots:
    void aceptar();
    void cancelar();
    void abrir();

    void histResult();
    void histEspecif();

signals:
    void changed();
    void acepted();

};

#endif // HESPECIF_H
