#ifndef RIMPULSIVO_H
#define RIMPULSIVO_H

#include <QWidget>
#include "imagen.h"
#include "Imagen/histograma.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>




namespace Ui {
    class RImpulsivo;
}

class RImpulsivo : public QWidget
{
    Q_OBJECT

public:
    explicit RImpulsivo(QWidget *parent = 0, Imagen image = Imagen());
    ~RImpulsivo();

    Imagen imagenAux; //imagen resultado de sumar el ruido a la imagen

private:
    Ui::RImpulsivo *ui;

    Imagen imagenR; //imagen ruido
    Imagen imagenOriginal;
    QLabel labelRuido; //label donde se vera la imagen ruido de forma sencilla

    int NO_RUIDO;
    int ** matRuido; //matriz que representa la imagen ruido
    bool imagenRuidoCreada;
    int F; //frecuencia absoluta de cada valor en posicion aleatoria

    void setValues(Imagen image);
    void crearImagenRuido(); //crea a partir de matriz de ruido. solo sirve para slots verImagenRuido y verHistRuido

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();

    void verImagenRuido(); //muestra imagen ruido
    void verHistRuido(); //muestra histograma de imagen ruido

signals:
    void changed();
    void acepted();
};

#endif // RIMPULSIVO_H
