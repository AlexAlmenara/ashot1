#include "rotacion.h"
#include "ui_rotacion.h"

#include <QGenericMatrix>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

Rotacion::Rotacion(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Rotacion)
{
    ui->setupUi(this);

    imagenOrig = imagen;
    imagenAux = imagen;

    ui->dSpinBoxAngulo->setRange(-365.0, 365.0);
    ui->dSpinBoxAngulo->setValue(0.0);
    ui->hSliderAngulo->setRange(-365, 365);
    ui->hSliderAngulo->setValue(0);

    ui->comboBoxInterpolacion->setCurrentIndex(BILINEAL);

    //conexiones
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->hSliderAngulo, SIGNAL(valueChanged(int)), this, SLOT(intToDouble(int)));
    connect(this, SIGNAL(updateSpinBox(double)), ui->dSpinBoxAngulo, SLOT(setValue(double)));

    connect(ui->dSpinBoxAngulo, SIGNAL(valueChanged(double)), this, SLOT(DoubleToInt(double)));
    connect(this, SIGNAL(updateSlider(int)), ui->hSliderAngulo, SLOT(setValue(int)));


    //cambios de slider y spinbox cambian imageLabel directamente
    connect(ui->dSpinBoxAngulo, SIGNAL(valueChanged(double)), this, SLOT(cambiarImagen()));
}

Rotacion::~Rotacion()
{
    delete ui;
}

void Rotacion::intToDouble(int value) {
    double convertedValue = (double) value;
    emit(updateSpinBox(convertedValue)); //se emite signal
}

void Rotacion::DoubleToInt(double value) {
    int convertedValue = (int) value;
    emit(updateSlider(convertedValue)); //se emite signal
}

void Rotacion::cambiarImagen() {
    //cout << "\ncambiar imagen" << flush;
    imagenAux = imagenOrig;
    double ang = ui->dSpinBoxAngulo->value(); //angulo

    //double * vect = new double [4];
    //matriz: transformacion directa
    QGenericMatrix<2, 2, double> matTransf; //(vect); //(N = columna, M = fila)
    matTransf(0, 0) = cos(ang); matTransf(0, 1) = -sin(ang); //primera fila (i = fila, j = columna)
    matTransf(1, 0) = sin(ang); matTransf(1, 1) = cos(ang);

    QGenericMatrix<1, 2, double> matMultp; //matriz columna que multiplica
    QGenericMatrix<1, 2, double> matResult; //matriz columna de resultado

    //calculo de esquinas
    //(0, 0) = E
    matMultp(0, 0) = imagenOrig.width() - 1; matMultp(1, 0) = 0;
    matResult = matTransf * matMultp;
    FPoint Ep; Ep.setX(matResult(0, 0)); Ep.setY(matResult(1, 0));
    //cout << "\nX = " << Ep.getX() << ", Y = " << Ep.getY() << endl;

    /*imagenAux.setQImage(imagenOrig.qimage.copy(QRect(0, 0, anchura, altura))); //copia imagen a partir de region de seleccion
    for (int Xp = 0; Xp < imagenAux.width(); Xp++) // reocorre la imagen escalada (mapeado inverso)
        for (int Yp = 0; Yp < imagenAux.height(); Yp++) { // (X', Y')
            double x = Xp / ui->dSpinBoxPAnchura->value();
            double y = Yp / ui->dSpinBoxPAltura->value();

            X = (int) x; //redondeo hacia abajo == floor(x);
            Y = (int) y;
            //tambien: X + 1, Y + 1

            double p = x - X;
            double q = y - Y;

            int gris;
            if (ui->comboBoxInterpolacion->currentIndex() == VMP) { //Vecino mas proximo
                //cout << "\nVMP" << flush;
                int vecinoX, vecinoY;
                if ((p > 0.5) && (X + 1 < imagenOrig.width())) //si se acerca mas a X+1 y X+1 no se sale de la imagen
                   vecinoX = X + 1;
                else
                    vecinoX = X;

                if ((q > 0.5) && (Y + 1 < imagenOrig.height())) //si se acerca mas a Y+1 y Y+1 no se sale de la imagen
                   vecinoY = Y + 1;
                else
                    vecinoY = Y;

                gris = imagenOrig.gray(vecinoX, vecinoY);
            }
            else { //Interpolacion Bilineal
                if ((X + 1 < imagenOrig.width()) && (Y + 1 < imagenOrig.height())) { //no se salen de la imagen
                    int B = imagenOrig.gray(X + 1, Y);
                    int C = imagenOrig.gray(X, Y + 1);
                    //cout << ", dentro todo" << flush;
                    int D = imagenOrig.gray(X + 1, Y + 1);
                    gris = (int) ((C + (D - C) * p + (A - C) * q + (B + C - A - D) * p * q) + 0.5); //redondeo al mas cercano
                }
                else
                    if ((X + 1 >= imagenOrig.width()) && (Y + 1 >= imagenOrig.height())) //se salen de la imagen
                        gris = A; //es solo (X, Y)
                    else
                        if (X + 1 >= imagenOrig.width()) { //X+1 se sale
                            int C = imagenOrig.gray(X, Y + 1);
                            gris = (int) ((A + (C - A) * q) + 0.5); //Y..Y+1
                        }
                        else {
                            int B = imagenOrig.gray(X + 1, Y);
                            gris = (int) ((A + (B - A) * p) + 0.5); //X..X+1 (Y+1 se sale)
                        }
            } //fin bilineal

            imagenAux.qimage.setPixel(Xp, Yp, gris);
        } //for */

    imagenAux.update();
    emit(changed());
}

void Rotacion::aceptar() {
    cambiarImagen();
    //aShot actualiza y despues:
    close();
    emit(acepted());
    delete this;
}

void Rotacion::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    delete this;
}
