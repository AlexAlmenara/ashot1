#include "fdef.h"
#include "ui_fdef.h"

FDef::FDef(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FDef)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));
    connect(ui->ButtonAplicar, SIGNAL(clicked()), this, SLOT(cambiarImagen()));

    connect(ui->ButtonCrearVentana, SIGNAL(clicked()), this, SLOT(crearVentana()));
    connect(ui->ButtonGuardar, SIGNAL(clicked()), this, SLOT(guardar()));
    connect(ui->ButtonAbrir, SIGNAL(clicked()), this, SLOT(abrir()));

    imagenOrig = image;
    imagenAux = image;

    ui->spinBoxTam->setRange(1, 99);
    ui->spinBoxTam->setValue(3); //por defecto ventana 3x3
}

FDef::~FDef() {
    delete ui;
}


void FDef::cambiarImagen() {
    imagenAux =  imagenOrig;


    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN. impar ya fue comprobado por crearVentana()

    //filtra
    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    int valor;
    for (int u = nborde; u <= imagenOrig.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOrig.height() - 1 - nborde; v++) {
            valor = imagenOrig.mediana(u - nborde, v - nborde,   u + nborde, v + nborde); //copia imagen a partir de region de seleccion, ya hace update()
            imagenAux.qimage.setPixel(u, v, valor);
        }

    imagenAux.update();
    emit(changed());
}

void FDef::aceptar() {
    cambiarImagen();
    this->close();
    emit(acepted());
    delete this;
}


void FDef::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    delete this;
}



void FDef::crearVentana() {
    //comprueba que sea tamaño impar y si no lo cambia
    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN
    if (!(N % 2)) { //no es impar
        N++;
        ui->spinBoxTam->setValue(N);
    }

    resize(N + 200, N + 200);

    boxFilas = new QHBoxLayout [N];
    ventana = new QSpinBox * [N]; //creacion de matriz de ruido. fuente: http://c.conclase.net/curso/?cap=017
    for (int i = 0; i < N; i++) {
        ventana[i] = new QSpinBox [N];

        for (int j = 0; j < N; j++) {
            ventana[i][j].setValue(1); //valor por defecto (filtro de la media)
            boxFilas[i].addWidget(&ventana[i][j]); //añade a hbox la fila
        }

        boxVentana.addLayout(&boxFilas[i]); //añade hbox de fila en la vbox
    }






}

void FDef::abrir() {

}


void FDef::guardar() {

}



