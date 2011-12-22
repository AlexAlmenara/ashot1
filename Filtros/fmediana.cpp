#include "fmediana.h"
#include "ui_fmediana.h"

FMediana::FMediana(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FMediana)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->spinBoxTam, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));

    imagenOriginal = image;
    imagenAux = image;

    ui->spinBoxTam->setRange(1, 99);
    ui->spinBoxTam->setValue(3); //por defecto ventana 3x3
}

FMediana::~FMediana()
{
    delete ui;
}

void FMediana::cambiarImagen() {
    imagenAux =  imagenOriginal;

    //comprueba que sea tamaño impar y si no lo cambia
    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN
    if (!(N % 2)) { //no es impar
        N++;
        ui->spinBoxTam->setValue(N);
    }

    //filtra
    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    int valor;
    for (int u = nborde; u <= imagenOriginal.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOriginal.height() - 1 - nborde; v++) {
            valor = imagenOriginal.mediana(u - nborde, v - nborde,   u + nborde, v + nborde); //copia imagen a partir de region de seleccion, ya hace update()
            imagenAux.qimage.setPixel(u, v, valor);
        }

    imagenAux.update();
    emit(changed());
}

void FMediana::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void FMediana::cancelar() {
    imagenAux = imagenOriginal;
    close();
    emit(changed());
    delete this;
}
