#include "logexp.h"
#include "ui_logexp.h"
#include "math.h"

Logexp::Logexp(QWidget *parent, Imagen imagen, QLabel * label) :
    QWidget(parent),
    ui(new Ui::Logexp)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aplicarCambios()));


    //spinbox
    connect(ui->log1radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(hide()));
    connect(ui->log2radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(show()));
    connect(ui->log3radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(show()));

    connect(ui->exp1radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(hide()));
    connect(ui->exp2radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(show()));
    connect(ui->exp3radioButton, SIGNAL(clicked()), ui->kdoubleSpinBox, SLOT(show()));

    //label de k:
    connect(ui->log1radioButton, SIGNAL(clicked()), ui->klabel, SLOT(hide()));
    connect(ui->log2radioButton, SIGNAL(clicked()), ui->klabel, SLOT(show()));
    connect(ui->log3radioButton, SIGNAL(clicked()), ui->klabel, SLOT(show()));

    connect(ui->exp1radioButton, SIGNAL(clicked()), ui->klabel, SLOT(hide()));
    connect(ui->exp2radioButton, SIGNAL(clicked()), ui->klabel, SLOT(show()));
    connect(ui->exp3radioButton, SIGNAL(clicked()), ui->klabel, SLOT(show()));

    ui->kdoubleSpinBox->setRange(1.0, 10.0);
    ui->kdoubleSpinBox->setValue(1); //valor por defecto de k
    ui->log3radioButton->setChecked(true); //log3 sera la funcion por defecto
    imagenAux =  imagen;
    imageLabel = label; //se igualan los punteros: ambos apuntan a la misma label donde se pinta la imagen
}


Logexp::~Logexp() {
    delete ui;
}


//funciones de transformacion: vout[vin] = f(vin, k)
//aun no mira si se pasa del rango [0, M -1], se hace despues
int Logexp::log1(int vin) {
    double vout = ((double) (imagenAux.M()) - 1) / log((double) imagenAux.M()); // M-1/ln(M)
    vout *= log((double)(vin + 1));
    return (int) vout;
}

int Logexp::log2(int vin, int k) { //exp(x) = e^x
    double vout = 1.0 + (exp((double) k) - 1.0) * ((double) vin / (double) (imagenAux.M() - 1));
    //printf("nvout1: %d", vout);
    vout = log(vout); //log = ln
    vout *= ((double) (imagenAux.M() - 1) / (double) k);
    return (int) vout;
}

int Logexp::log3(int vin, int k) {
    double vout = 1.0 + (exp((double) k) - 1.0) * ((double) imagenAux.hAcumNorm(vin));
    vout = log(vout); //log = ln
    vout *= (double) (imagenAux.M() - 1) / (double) k;
    return (int) vout;
}

int Logexp::exp1(int vin) {
    int vout = imagenAux.negativo(vin);
    vout = log1(vout);
    vout = imagenAux.negativo(vout);
    return vout;
}

int Logexp::exp2(int vin, int k) {
    int vout = imagenAux.negativo(vin);
    vout = log2(vout, k);
    vout = imagenAux.negativo(vout);
    return vout;
}

int Logexp::exp3(int vin, int k) {
    int vout = imagenAux.negativo(vin);
    vout = log3(vout, k);
    vout = imagenAux.negativo(vout);
    return vout;
}

//slots:

void Logexp::aplicarCambios() {
    int * vout = new int [imagenAux.M()]; //tabla LUT
    int k = ui->kdoubleSpinBox->value();

    for (int vin = 0; vin < imagenAux.M(); vin++) {
        if (ui->log1radioButton->isChecked())
            vout[vin] = log1(vin);
        else
        if (ui->log2radioButton->isChecked())
        vout[vin] = log2(vin, k); //vin, k
        else
        if (ui->log3radioButton->isChecked())
        vout[vin] = log3(vin, k);
        else
        if (ui->exp1radioButton->isChecked())
        vout[vin] = exp1(vin);
        else
        if (ui->exp2radioButton->isChecked())
        vout[vin] = exp2(vin, k);
        else
        if (ui->exp3radioButton->isChecked())
            vout[vin] = exp3(vin, k);

        if (vout[vin] > imagenAux.M() - 1)
            vout[vin] = imagenAux.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }


    imagenAux.transformar(vout); //transformacion. tambien hace el update de histograma
    imageLabel->setPixmap(QPixmap::fromImage(imagenAux.qimage)); //shot->updateImageLabel();//actualiza label

    this->close();
    emit(closed()); //se emite signal
}


void Logexp::cancelar() {
    close();
    emit(closed()); //se emite signal
}



