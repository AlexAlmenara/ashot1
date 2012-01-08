#ifndef RGAUSSIANO_H
#define RGAUSSIANO_H

#include <QWidget>
#include <QtGui>
#include "imagen.h"
#include "Imagen/histograma.h"


#define PI 3.14159
//#define E 2,71828

namespace Ui {
    class RGaussiano;
}

class RGaussiano : public QWidget
{
    Q_OBJECT

public:
    explicit RGaussiano(QWidget *parent = 0, Imagen image = Imagen());
    ~RGaussiano();

    Imagen imagenAux; //imagen resultado de sumar el ruido a la imagen

private:
    Ui::RGaussiano *ui;


    Imagen imagenR; //imagen ruido
    Imagen imagenOriginal;
    QLabel labelRuido; //label donde se vera la imagen ruido de forma sencilla

    int ** matRuido; //matriz que representa la imagen ruido
    int Mnew; //el numero de valores diferentes para ruido
    bool imagenRuidoCreada;
    int NO_RUIDO; //"constante" = n1 - 1
    int N; //numero de pixeles ruidosos
    double S; //suma de los valores de fGauss desde n1 a n2

    void setValues(Imagen image);
    void crearImagenRuido(); //crea a partir de matriz de ruido. solo sirve para slots verImagenRuido y verHistRuido

    double fGauss(double x); //funcion de gauss (los parametros media y desv tipica ya los coge dentro)
    int F(int x); //frecuencia absoluta del valor de ruido x

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

#endif // RGAUSSIANO_H
