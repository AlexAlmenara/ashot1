#include "fdifest.h"
#include "ui_fdifest.h"

FDifest::FDifest(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FDifest)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->spinBoxTam, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->doubleSpinBoxContraste, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));

    imagenOriginal = image;
    imagenAux = image;

    ui->spinBoxTam->setRange(1, 99);
    ui->spinBoxTam->setValue(3); //por defecto ventana 3x3
    ui->doubleSpinBoxContraste->setRange(0.0, ((double) imagenOriginal.M() / 2.0) );
    ui->doubleSpinBoxContraste->setValue(imagenOriginal.contraste());
}

FDifest::~FDifest()
{
    delete ui;
}

void FDifest::cambiarImagen() {
    imagenAux =  imagenOriginal;

    //comprueba que sea tamaño impar y si no lo cambia
    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN
    if (!(N % 2)) { //no es impar
        N++;
        ui->spinBoxTam->setValue(N);
    }

    double newContr = ui->doubleSpinBoxContraste->value(); //contraste que especifica usuario
    double localContr; //contraste local de una ventana
    double localBri; //brillo (media) local de una ventana

    //filtra
    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    int valor;
    for (int u = nborde; u <= imagenOriginal.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOriginal.height() - 1 - nborde; v++) {
            localBri = imagenOriginal.brillo(u - nborde, v - nborde,   u + nborde, v + nborde);
            localContr = imagenOriginal.contraste(u - nborde, v - nborde,   u + nborde, v + nborde);

            valor = localBri + (imagenOriginal.gray(u, v) - localBri) * (newContr / localContr);
            if (valor < 0)
                valor = 0;
            else
                if (valor > imagenAux.M() - 1)
                    valor = imagenAux.M() - 1;

            imagenAux.qimage.setPixel(u, v, valor);
        }

    imagenAux.update();
    emit(changed());
}

void FDifest::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void FDifest::cancelar() {
    imagenAux = imagenOriginal;
    close();
    emit(changed());
    delete this;
}
