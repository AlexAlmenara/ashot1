#include "hespecif.h"
#include "ui_hespecif.h"
#include <stdio.h>
#include <stdlib.h>

Hespecif::Hespecif(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::Hespecif)
{
    ui->setupUi(this);

    move(600, 0);
    imagenOriginal = image;
    imagenAux = image;

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));
    connect(ui->ButtonAbrir, SIGNAL(clicked()), this, SLOT(abrir()));

    connect(ui->checkBoxhResult, SIGNAL(pressed()), this, SLOT(histResult()));
    connect(ui->checkBoxhSelect, SIGNAL(pressed()), this, SLOT(histEspecif()));

    abrir();
}

Hespecif::~Hespecif()
{
    delete ui;
}



double Hespecif::distancia(int vin0, int vin1) { //niveles: vin0 de imagen, vin1 de imagenEsp
    return fabs(imagenAux.hAcumNorm(vin0) - imagenEsp.hAcumNorm(vin1));
}


int Hespecif::minDist(int vin0) { //devuelve nivel vin1 de imagenEsp, tal que sea minima distncia de vin0 de imagen
    int vin1 = 0; //el vin de imagenEsp a devolver
    for (int vin1_it = 0; vin1_it < imagenEsp.M(); vin1_it++) //recorre los niveles de la imagen seleccionada
        if (distancia(vin0, vin1_it) < distancia(vin0, vin1))
            vin1 = vin1_it; //se busca el de menor distancia

    return vin1;
}



void Hespecif::updateLabel() {
    //label.hide();
    label.setPixmap(QPixmap::fromImage(imagenEsp.qimage));
    //label.resize(imagenEsp.width(), imagenEsp.height());
    label.show();
}



void Hespecif::aceptar() {
    this->close();
    emit(changed()); //no hace falta, ya lo hace abrir
    emit(acepted());
    delete this;
}


void Hespecif::cancelar() {
    imagenAux = imagenOriginal;
    close();
    emit(changed());
    delete this;
}


void Hespecif::abrir() { //reinicia, abre otra imagen y ejecuta la conversion
    imagenAux = imagenOriginal;

    //abrir imagen
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        imagenEsp = Imagen(fileName);
        if (imagenEsp.isNull()) {
            QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
            return;
        }

        updateLabel();

        //transformacion de imagen de trabajo
        int * vout = new int [imagenAux.M()]; //tabla LUT

        for (int vin = 0; vin < imagenAux.M(); vin++) {
            vout[vin] = minDist(vin);
        }

        imagenAux.transformar(vout); //transformacion. tambien hace el update de histograma
        //printf("eyy changeddd");
        //emit(changed());
    } //if

    //emit(changed());
}


void Hespecif::histResult() {
    Histograma * histograma = new Histograma(0, imagenAux);
    histograma->show();
    histograma->move(600, 500);
}


void Hespecif::histEspecif() {
    Histograma * histograma = new Histograma(0, imagenEsp);
    histograma->show();
    histograma->move(1020, 500);
}

