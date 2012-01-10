#include "escalado.h"
#include "ui_escalado.h"

#include <stdio.h>
#include <math.h>

#include <iostream>

using namespace std;

Escalado::Escalado(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Escalado)
{
    ui->setupUi(this);

    imagenOrig = imagen;
    imagenAux = imagen;

    //dimensiones
    ui->spinBoxAnchura->setRange(1, imagenOrig.width() * MAX_MULTP);
    ui->spinBoxAnchura->setValue(imagenOrig.width() );
    ui->spinBoxAltura->setRange(1, imagenOrig.height() * MAX_MULTP);
    ui->spinBoxAltura->setValue(imagenOrig.height());

    // % de dimensiones
    ui->dSpinBoxPAnchura->setRange(0.01, MAX_MULTP);
    ui->dSpinBoxPAnchura->setValue(1); //100 % por defecto: la imagen igual
    ui->dSpinBoxPAltura->setRange(0.01, MAX_MULTP);
    ui->dSpinBoxPAltura->setValue(1); //100 % por defecto: la imagen igual

    ui->comboBoxInterpolacion->setCurrentIndex(BILINEAL);

    //conexiones
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));
    connect(ui->ButtonAplicar, SIGNAL(clicked()), this, SLOT(cambiarImagen()));

    connect(ui->spinBoxAltura, SIGNAL(editingFinished()), this, SLOT(setDimensiones()));
    connect(ui->spinBoxAnchura, SIGNAL(editingFinished()), this, SLOT(setDimensiones()));
    connect(ui->dSpinBoxPAltura, SIGNAL(editingFinished()), this, SLOT(setPDimensiones()));
    connect(ui->dSpinBoxPAnchura, SIGNAL(editingFinished()), this, SLOT(setPDimensiones()));
    //connect(ui->comboBoxInterpolacion, SIGNAL(currentIndexChanged(int)), this, SLOT(cambiarImagen()));
}

Escalado::~Escalado()
{
    delete ui;
}


void Escalado::setDimensiones() { //actualiza valores segun spinBox de dimensiones
    //cout << "dim" << flush;
    anchura = ui->spinBoxAnchura->value();
    altura = ui->spinBoxAltura->value();

    //halla porcentajes por regla de tres
    ui->dSpinBoxPAnchura->setValue((double) anchura / (double) imagenOrig.width());
    ui->dSpinBoxPAltura->setValue((double) altura / (double) imagenOrig.height());
}

void Escalado::setPDimensiones() { //actualiza valores segun dSpinBox de porcentajes
    //halla dimensiones por regla de tres
    anchura = (int) ((imagenOrig.width() * ui->dSpinBoxPAnchura->value()) + 0.5);
    altura = (int) ((imagenOrig.height() * ui->dSpinBoxPAltura->value()) + 0.5);

    ui->spinBoxAnchura->setValue(anchura);
    ui->spinBoxAltura->setValue(altura);
}

void Escalado::cambiarImagen() {
    //cout << "\ncambiar imagen" << flush;
    imagenAux = imagenOrig;

    //cambio

    //imagenAux = Imagen(QImage(width, height, QImage::Format_Indexed8), imagenOriginal.path()); //es igual que poner formato = 3
    /*imagenAux.setQImage(imagenOrig.qimage.copy(QRect(0, 0, anchura, altura))); //copia imagen a partir de region de seleccion

    int value;
    double xdist = ((double) imagenOrig.width() / (double) anchura);
    double ydist = ((double) imagenOrig.height() / (double) altura);

    for (int i = 0; i < imagenAux.width(); i++) // reocorre la imagen escalada (mapeado inverso)
        for (int j = 0; j < imagenAux.height(); j++) {
            value = imagenOrig.gray((int) i * xdist, (int) j * ydist); //muestreo
            //printf("\nvalue: %d", value);
            imagenAux.qimage.setPixel(i, j, value);
        }*/

    imagenAux.setQImage(imagenOrig.qimage.copy(QRect(0, 0, anchura, altura))); //copia imagen a partir de region de seleccion
    for (int Xp = 0; Xp < imagenAux.width(); Xp++) // reocorre la imagen escalada (mapeado inverso)
        for (int Yp = 0; Yp < imagenAux.height(); Yp++) { // (X', Y')
            double x = Xp / ui->dSpinBoxPAnchura->value();
            double y = Yp / ui->dSpinBoxPAltura->value();

            int X = (int) x; //redondeo hacia abajo == floor(x);
            int Y = (int) y;
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
                int A = imagenOrig.gray(X, Y);
                /* int B = -1, C = -1, D  = -1;
                if (X + 1 < imagenOrig.width())
                    B = imagenOrig.gray(X + 1, Y);

                if (Y + 1 < imagenOrig.height())
                    C = imagenOrig.gray(X, Y + 1);*/

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
        } //for

    imagenAux.update();
    emit(changed());
}

void Escalado::aceptar() {
    cambiarImagen();
    //aShot actualiza y despues:
    close();
    emit(acepted());
    delete this;
}

void Escalado::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    delete this;
}
