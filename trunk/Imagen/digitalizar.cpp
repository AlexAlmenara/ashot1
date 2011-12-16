#include "digitalizar.h"
#include "ui_digitalizar.h"

Digitalizar::Digitalizar(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Digitalizar)
{
    ui->setupUi(this);

    imagenOriginal = imagen; //imagenAux aun no, se crea en aceptar()
    imagenAux = imagen;

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    ui->spinBoxNbits->setRange(1, 7);
    ui->spinBoxAnchura->setRange(20, imagen.width() - 1); //minimo por ejemplo 20 pixeles
    ui->spinBoxAltura->setRange(20, imagen.height() - 1); //maximo: debe ser menor que la imagen original

    ui->spinBoxAnchura->setValue(imagen.width() / 2); //200
    ui->spinBoxAltura->setValue(imagen.height() / 2); //320
    ui->spinBoxNbits->setValue(7);
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
    //imagenAux = Imagen(QImage(width, height, QImage::Format_Indexed8), imagenOriginal.path()); //es igual que poner formato = 3
    imagenAux.setQImage(imagenOriginal.qimage.copy(QRect(0, 0, width, height))); //copia imagen a partir de region de seleccion

    int value;
    double xdist = ((double) imagenOriginal.width() / (double) width);
    double ydist = ((double) imagenOriginal.height() / (double) height);

    //if (imagenOriginal.qimage.format() == QImage::Format_Indexed8)
      //  printf("original siiiiii");

    for (int i = 0; i < imagenAux.width(); i++) // igual: qimage.width()
        for (int j = 0; j < imagenAux.height(); j++) {
            value = imagenOriginal.perfil((int) i * xdist, (int) j * ydist); //muestreo
            //printf("\nvalue: %d", value);
            imagenAux.qimage.setPixel(i, j, value);
        }


    //cuantizacion
    int newM = (int) pow((double) nBits, 2.0); //nuevo numero de niveles
    double mdist = (double) newM / (double) imagenOriginal.M(); //podria ser tambien (Mn - 1) /(M - 1)
    int * vout = new int [imagenAux.M()]; //tabla LUT

    for (int vin = 0; vin < imagenAux.M(); vin++)
        vout[vin] = (int) (mdist * vin); //cuantizacion tipo 1
        //vout[vin] = (int)


    imagenAux.transformar(vout); //transformacion. tambien hace el update de */


    this->close();
    emit(changed()); //se emite signal
}
