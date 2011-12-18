#include "digitalizar.h"
#include "ui_digitalizar.h"

Digitalizar::Digitalizar(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Digitalizar)
{
    ui->setupUi(this);

    imagenOriginal = imagen;
    imagenAux = imagen;

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    ui->spinBoxNbits->setRange(1, 8);
    ui->spinBoxAnchura->setRange(20, imagen.width()); //minimo por ejemplo 20 pixeles
    ui->spinBoxAltura->setRange(20, imagen.height()); //maximo: maxima calidad de muestreo: exactamente la misma imagen
    ui->spinBoxAnchura->setValue(imagen.width()); //por defecto maxima calidad
    ui->spinBoxAltura->setValue(imagen.height()); //320
    ui->spinBoxNbits->setValue(8); //por defecto maxima calidad de cuantizacion: la misma imagen orignal
}

Digitalizar::~Digitalizar()
{
    delete ui;
}


void Digitalizar::cancelar() {
    close();
    emit(changed());
}


void Digitalizar::aceptar() {
    //asignacion de valores introducidos por el usuario
    nBits = ui->spinBoxNbits->value();
    width = ui->spinBoxAnchura->value();
    height = ui->spinBoxAltura->value();

    //muestreo
    //imagenAux.setQImage(imagenOriginal.qimage.copy(QRect(0, 0, width, height))); //copia imagen a partir de region de seleccion


    int value;
    double xdist = ((double) imagenOriginal.width() / (double) width);
    double ydist = ((double) imagenOriginal.height() / (double) height);

    for (int i = 0; i < width; i++) // recorremos la imagen por rejillas
        for (int j = 0; j < height; j++)
            for (int x = i * xdist; x < i * xdist + xdist - 1; x++)  //recorremos los pixeles de la rejilla
                for (int y = j * ydist; y < j * ydist + ydist - 1; y++) {
                    value = imagenOriginal.gray((int) i * xdist, (int) j * ydist); //muestreo
                    //printf("\nvalue: %d", value);
                    imagenAux.qimage.setPixel(x, y, value);
                }



    //cuantizacion
    if (nBits < 8) { //con 8 bits se puede hacer, pero no vale la pena porque no cambia nada
        int newM = (int) pow(2.0, (double) nBits); //nuevo numero de niveles
        double mdist1 = (double) (newM - 1) / (double) (imagenOriginal.M() - 1);
        double mdist2 = ((double) imagenOriginal.M() - 1) / ((double) newM - 1);
        int * vout = new int [imagenAux.M()]; //tabla LUT

        for (int vin = 0; vin < imagenAux.M(); vin++) {
            vout[vin] = (int) (mdist1 * vin + 0.5); //cuantizacion: acorta el rango. se pierde informacion al redondear. 0.5 para redondear
            vout[vin] = (int) (vout[vin] * mdist2 + 0.5); //se vuelve al rango [0..255] porque esto es solo una simulacion
            /*if (vout[vin] > imagenAux.M() - 1)
                vout[vin] = imagenAux.M() - 1;
            else
                if (vout[vin] < 0)
                    vout[vin] = 0;*/
         }

        imagenAux.transformar(vout); //transformacion. tambien hace el update de imagen
    } //if

    this->close();
    emit(changed()); //se emite signal
}



/*
//esto es lo que seria mas o menos el ESCALADO a una menor escala

    //imagenAux = Imagen(QImage(width, height, QImage::Format_Indexed8), imagenOriginal.path()); //es igual que poner formato = 3
    imagenAux.setQImage(imagenOriginal.qimage.copy(QRect(0, 0, width, height))); //copia imagen a partir de region de seleccion

    //muestreo
    int value;
    double xdist = ((double) imagenOriginal.width() / (double) width);
    double ydist = ((double) imagenOriginal.height() / (double) height);

    //if (imagenOriginal.qimage.format() == QImage::Format_Indexed8)
      //  printf("original siiiiii");

    for (int i = 0; i < imagenAux.width(); i++) // igual: qimage.width()
        for (int j = 0; j < imagenAux.height(); j++) {
            value = imagenOriginal.gray((int) i * xdist, (int) j * ydist); //muestreo
            //printf("\nvalue: %d", value);
            imagenAux.qimage.setPixel(i, j, value);
        }

*/



