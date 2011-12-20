#include "rgaussiano.h"
#include "ui_rgaussiano.h"

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

RGaussiano::RGaussiano(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::RGaussiano)
{
    ui->setupUi(this);

    setValues(image);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    //cambios de spinbox cambian imagen directamente
    connect(ui->doubleSpinBoxP, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->spinBoxN1, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->spinBoxN2, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->doubleSpinBoxMedia, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->doubleSpinBoxDesv, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));

    connect(ui->checkBoxHistR, SIGNAL(pressed()), this, SLOT(verHistRuido()));
    connect(ui->checkBoxImagenR, SIGNAL(pressed()), this, SLOT(verImagenRuido()));


    connect(ui->spinBoxN1, SIGNAL(editingFinished()), this, SLOT(comprobarRango()));
    connect(ui->spinBoxN2, SIGNAL(editingFinished()), this, SLOT(comprobarRango()));
    //cambiarImagen(); //por defecto ya se ve un ruido hecho
    //emit(changed());
}

RGaussiano::~RGaussiano()
{
    delete ui;
}


void RGaussiano::setValues(Imagen image) {
    imagenOriginal = image;
    imagenAux = image;
    imagenRuidoCreada = false;
    matRuido = new int * [imagenOriginal.width()]; //creacion de matriz de ruido. fuente: http://c.conclase.net/curso/?cap=017
    for (int i = 0; i < imagenOriginal.width(); i++)
        matRuido[i] = new int[imagenOriginal.height()];

    ui->doubleSpinBoxP->setRange(0.0, 100.0); //porcentaje de contaminacion
    ui->doubleSpinBoxP->setValue(10.0);

    ui->spinBoxN1->setRange(-(imagenOriginal.M() - 1), imagenOriginal.M() - 2); //-255..254
    ui->spinBoxN1->setValue(-50);
    ui->spinBoxN2->setRange(-(imagenOriginal.M() - 2), imagenOriginal.M() - 1); //-254..255
    ui->spinBoxN2->setValue(50);

    ui->doubleSpinBoxMedia->setRange(-(double) (imagenOriginal.M() - 1), (double) (imagenOriginal.M() - 1));
    ui->doubleSpinBoxMedia->setValue(20);
    ui->doubleSpinBoxDesv->setRange(0.0, ((double) imagenOriginal.M() / 2.0));
    ui->doubleSpinBoxDesv->setValue(5);
}


void RGaussiano::comprobarRango() {
    if (ui->spinBoxN1->value() >= ui->spinBoxN2->value())
        ui->spinBoxN1->setValue(-(imagenOriginal.M() - 1)); //se pone el minimo valor
}


double RGaussiano::fGauss(double x) { //funcion de gauss (los parametros media y desv tipica ya los coge dentro)
    double valor = x - ui->doubleSpinBoxMedia->value();
    valor = pow(valor, 2) / (2.0 * pow(ui->doubleSpinBoxDesv->value(), 2));
    valor = exp(-valor);     //exp(x) = e^x
    valor *= 1.0 / (ui->doubleSpinBoxDesv->value() * sqrt(2.0 * PI));
    return valor;
}


int RGaussiano::F(int x) { //frecuencia absoluta del valor de ruido x
    return (int) ((fGauss(x) * (((double) N) / S)) + 0.5);
}


void RGaussiano::cambiarImagen() {
    //inicializar valores
    imagenAux =  imagenOriginal;
    srand(time(NULL)); //reinicia semilla aleatoria

    double p = ui->doubleSpinBoxP->value(); //porcentaje de contaminacion
    N = (int) ((p / 100.0) * (double) imagenOriginal.size()); //numero de pixeles ruidosos a generar
    int n1 = ui->spinBoxN1->value(); //rango [n1, n2]
    int n2 = ui->spinBoxN2->value();
    Mnew = n2 - n1 + 1;

    //determinar S
    S = 0;
    for (int vin = n1; vin <= n2; vin++) //recorre el rango de valores aleatorios
        S += fGauss(vin);


    NO_RUIDO = n1 - 1;
    //inicializar matriz ruido
    //matRuido = new int [imagenOriginal.width()][imagenOriginal.height()]; //QGenericMatrix<W, H, int> matRuido;
    for (int i = 0; i < imagenOriginal.width(); i++)
        for (int j = 0; j < imagenOriginal.height(); j++)
            matRuido[i][j] = NO_RUIDO; //valor de NO RUIDO

    //poner ruido en casillas aleatorias de matriz ruido
    int x, y;
    for (int vin = n1; vin <= n2; vin++) //recorre el rango de valores aleatorios
        for (int j = 0; j < F(vin); j++) { //F veces por nivel
            do { //encontrar una casilla aleatoria aun sin ruido
                x = rand() % (imagenOriginal.width() - 1); //0..width-1
                y = rand() % (imagenOriginal.height() - 1); //0..width-1
            }
            while (matRuido[x][y] != NO_RUIDO); //con el do..while, obliga a que se ejecuta al menos una vez (como el repeat)

            matRuido[x][y] = vin; //en posicion aleatoria se asigna este valor
        }


    //añadir ruido a imagen
    int valor;
    for (int i = 0; i < imagenAux.width(); i++) //es lo mismo que con imagenOriginal
        for (int j = 0; j < imagenAux.height(); j++) {
            if (matRuido[i][j] != NO_RUIDO) {
                valor = imagenAux.gray(i, j) + matRuido[i][j]; //añadimos ruido
                if (valor > imagenAux.M() - 1)
                    valor = imagenAux.M() - 1;
                else
                    if (valor < 0)
                        valor = 0;

                //printf(", valor: %d", valor);
                imagenAux.qimage.setPixel(i, j, valor);
            }
         }

    imagenAux.update();
    imagenRuidoCreada = false; //aun no se ha creado para mostrar
    emit(changed()); //se ha cambiado algo, se lo comunica a aShot para actualizar imageLabel //imageLabel->setPixmap(QPixmap::fromImage(imagenAux.qimage)); //shot->updateImageLabel();//actualiza label
}




void RGaussiano::crearImagenRuido() { //crea a partir de matriz de ruido. solo sirve para slots verImagenRuido y verHistRuido
    int valor;
    int n1 = ui->spinBoxN1->value(); //no hace falta n2

    //el rango de la imagenR sera 0..254, porque el 255 lo reservamos para valor NO_RUIDO
    double dist = ((double) (imagenOriginal.M() - 2) / (double) (Mnew - 1));
    imagenR = imagenOriginal; //solo para las dimensiones

    imagenR.qimage.setColor(imagenR.M() - 1, qRgb(0, 0, 255)); //el 255 lo reservamos para NO_RUIDO: color azul
    for (int i = 0; i < imagenR.width(); i++) //es lo mismo que con imagenOriginal
        for (int j = 0; j < imagenR.height(); j++) {
            if (matRuido[i][j] == NO_RUIDO)
                valor = imagenR.M() - 1; //el 255 lo reservamos para NO_RUIDO
            else {
                valor = matRuido[i][j];
                if (n1 != 0) valor -= n1; //pasamos rango [n1, n2] a [0, n2-n1]
                valor = (int) (valor * dist + 0.5); //pasamos rango a [0..254]
            }

            imagenR.qimage.setPixel(i, j, valor);
        }

    imagenR.update();
}


void RGaussiano::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void RGaussiano::cancelar() {
    imagenAux = imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    close();
    emit(changed());
    delete this;
}


void RGaussiano::verHistRuido() {
    if (!imagenRuidoCreada) {
        crearImagenRuido();
        imagenRuidoCreada = true;
    }

    int n1 = ui->spinBoxN1->value(); //no hace falta n2
    double dist = ((double) (imagenOriginal.M() - 2) / (double) (Mnew - 1));
    double media = ui->doubleSpinBoxMedia->value();
    if (n1 != 0) media -= n1; //pasamos rango [n1, n2] a [0, n2-n1]
    media = (int) (media * dist + 0.5); //pasamos rango a [0..254]

    int hmedia = imagenR.hAbs(media);
    Histograma * histograma = new Histograma(0, imagenR, hmedia + 40); //cambiamos el rango de grises: 0..254, xq el 255 lo reservamos para NO_RUIDO

    histograma->show();
    histograma->move(1100, 500);
}


void RGaussiano::verImagenRuido() {
    if (!imagenRuidoCreada) {
        crearImagenRuido();
        imagenRuidoCreada = true;
    }

    labelRuido.setPixmap(QPixmap::fromImage(imagenR.qimage));
    labelRuido.move(this->x() + 600, this->y());
    labelRuido.show();
}

