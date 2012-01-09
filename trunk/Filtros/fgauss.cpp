#include "fgauss.h"
#include "ui_fgauss.h"


#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

FGauss::FGauss(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FGauss)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->dSpinBoxDesv, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));

    imagenOrig = image;
    imagenAux = image;

    ui->dSpinBoxDesv->setRange(0.0, ((double) imagenOrig.M() / 2.0));
    ui->dSpinBoxDesv->setValue(1.5);
}

FGauss::~FGauss()
{
    delete ui;
}


double FGauss::fG(double x) { //funcion de gauss (los parametros media y desv tipica ya los coge dentro)
    double valor = pow(x, 2) / (2 * pow(ui->dSpinBoxDesv->value(), 2));
    valor = 1000 * exp(-valor);
    return valor;
}


void FGauss::cambiarImagen() {
    imagenAux =  imagenOrig;

    //calculo del kernel
    int w = (int) (7.4 * ui->dSpinBoxDesv->value()); //tamaño del kernel unidimensional
    if (!(w % 2)) //si no es impar
        w++; //lo pone impar

    ui->labelW->setText("Tamano del kernel calculado: " + QString::number(w));
    int nborde = w / 2; //redondeado hacia abajo
    //printf("\n nborde= %d", nborde);
    int * kernel = new int [w]; //kernel unidimensional (para x e y es el mismo)
    for (int x = 0; x <= nborde; x++) {
        kernel[w/2 + x] = (int) (fG(x) + 0.5);
        kernel[w/2 - x] = (int) (fG(x) + 0.5);
    }

    //printf("\nw = %d\n", w);
    double norm = 0; //factor de normalizacion
    for (int i = 0; i < w; i++) {
        //printf(", %d", kernel[i]);
        norm += kernel[i];
    }
    norm = 1.0 / norm;
    //printf("\nnorm = %f", norm);


    //primera convolucion unidimensional: x (recorre width)
    Imagen imagenAux1 = imagenOrig;
    for (int u = nborde; u <= imagenOrig.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = 0; v <= imagenOrig.height() - 1; v++) {
            int media = 0; //valor a asignar a pixel (u, v)

            for (int i = -nborde; i <= nborde; i++) { //recorre kernel (ventana)
                    int gris = imagenOrig.gray(u + i, v);
                    media += gris * kernel[i + nborde];
            }

            media = (int) (( ((double) media) * norm) + 0.5);
            imagenAux1.qimage.setPixel(u, v, media);
        }
    imagenAux1.update();

    //segunda convolucion unidimensional: y (recorre height)
    for (int u = 0; u <= imagenAux1.width() - 1; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenAux1.height() - 1 - nborde; v++) {
            int media = 0; //valor a asignar a pixel (u, v)

            for (int j = -nborde; j <= nborde; j++) { //recorre kernel (ventana)
                    int gris = imagenAux1.gray(u, v + j);
                    media += gris * kernel[j + nborde];
            }

            media = (int) (( ((double) media) * norm) + 0.5);
            imagenAux.qimage.setPixel(u, v, media);
        }

    imagenAux.update();
    emit(changed());
}

void FGauss::aceptar() {
    //cambiarImagen();
    this->close();
    emit(acepted());
    delete this;
}


void FGauss::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    delete this;
}

