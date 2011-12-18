#ifndef DIFERENCIA_H
#define DIFERENCIA_H

#include <QWidget>
#include "Imagen/histograma.h"
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
    class Diferencia;
}

class Diferencia : public QWidget
{
    Q_OBJECT

public:
    explicit Diferencia(QWidget *parent = 0, Imagen image = Imagen());
    ~Diferencia();

    Imagen imagenAux; //imagen de trabajo a cambiar, esta es la imagen DIFERENCIA

private:
    Ui::Diferencia *ui;

    QLabel label1, label2; //ERROR DE QT: si pongo esta label1 debajo o al lado de label, petaaa
    Imagen imagenOriginal; //I1
    Imagen imagenEsp; //I2: imagen elegida para copiar su histograma a imagen
    QLabel label; //label donde se vera la nueva imagen. atencion saldria sencillo, no en un aShot nuevo

    void updateLabel();
    int dif(int x, int y); //diferencia entre los dos pixeles de las imagens I1 y I2 en posicion (x, y)

public slots:
    void aceptar();
    void cancelar();
    void abrir();

    void histResult(); //saca histograma del resultado de la diferencia
    void histEspecif(); //saca histograma de la imagen especificada (I2)
    void i1_cambios(); //muestra imagen I1 (imagenOriginal) con pixeles de cambio marcados
    void i2_cambios(); //idem pero con imagen I2 (imagen especificada)

signals:
    void changed();
    void acepted();
};

#endif // DIFERENCIA_H
