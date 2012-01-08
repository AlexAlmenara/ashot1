#ifndef RUNIFORME_H
#define RUNIFORME_H

#include <QWidget>
#include <QtGui>
#include "imagen.h"
#include "Imagen/histograma.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace Ui {
    class RUniforme;
}

class RUniforme : public QWidget
{
    Q_OBJECT

public:
    explicit RUniforme(QWidget *parent = 0, Imagen image = Imagen());
    ~RUniforme();

    Imagen imagenAux; //imagen resultado de sumar el ruido a la imagen

private:
    Ui::RUniforme *ui;


    Imagen imagenR; //imagen ruido
    Imagen imagenOriginal;
    QLabel labelRuido; //label donde se vera la imagen ruido de forma sencilla

    int ** matRuido; //matriz que representa la imagen ruido
    int Mnew; //el numero de valores diferentes para ruido
    bool imagenRuidoCreada;
    int NO_RUIDO; //"constante" = n1 - 1
    int F; //frecuencia absoluta de cada valor en posicion aleatoria

    void setValues(Imagen image);
    void crearImagenRuido(); //crea a partir de matriz de ruido. solo sirve para slots verImagenRuido y verHistRuido

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();

    void verImagenRuido(); //muestra imagen ruido
    void verHistRuido(); //muestra histograma de imagen ruido
    void comprobarRango();

signals:
    void changed();
    void acepted();

};

#endif // RUIDOUNIFORME_H
