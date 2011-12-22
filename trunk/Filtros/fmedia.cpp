#include "fmedia.h"
#include "ui_fmedia.h"

FMedia::FMedia(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FMedia)
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

FMedia::~FMedia()
{
    delete ui;
}


void FMedia::cambiarImagen() {
    imagenAux =  imagenOriginal;

    //comprueba que sea tamaño impar y si no lo cambia
    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN
    if (!(N % 2)) { //no es impar
        N++;
        ui->spinBoxTam->setValue(N);
    }

    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    for (int u = nborde; u <= imagenOriginal.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOriginal.height() - 1 - nborde; v++) {
            int media = 0; //valor de la media

            for (int i = -nborde; i <= nborde; i++) //recorre kernel (ventana)
                for (int j = -nborde; j <= nborde; j++) {
                    int gris = imagenOriginal.gray(u + i, v + j);
                    media += gris;
                }

            media = (int) (((double) media / (double) (N * N)) + 0.5);
            imagenAux.qimage.setPixel(u, v, media);
        }

    imagenAux.update();
    emit(changed());
}

void FMedia::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void FMedia::cancelar() {
    imagenAux = imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    close();
    emit(changed());
    delete this;
}
