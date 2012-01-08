#include "fdef.h"
#include "ui_fdef.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

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
    N = 3;

    ui->dSpinBoxNorm->setRange(-100.0, 100.0);
    ui->dSpinBoxNorm->setValue(1.0/9.0); //factor de normalizacion: por defecto: filtro media
    firstPainted = false;
    crearVentana(); //por defecto filtro media
    firstPainted = true;
}

FDef::~FDef() {
    delete ui;
}


void FDef::cambiarImagen() {
    imagenAux =  imagenOrig;

    //filtra
    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    for (int u = nborde; u <= imagenOrig.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOrig.height() - 1 - nborde; v++) {
            int media = 0; //valor de la media

            for (int i = -nborde; i <= nborde; i++) //recorre kernel (ventana)
                for (int j = -nborde; j <= nborde; j++) {
                    int gris = imagenOrig.gray(u + i, v + j);
                    media = media + gris * ventana[i + nborde][j + nborde].value(); //pixel * elemento ventana
                }

            media = (int) (( ((double) media) * ui->dSpinBoxNorm->value()) + 0.5); //multiplica por valor de normalizacion (redondeo cercano)
            if (media > imagenOrig.M() - 1)
                media = imagenOrig.M() - 1;
            else
                if (media < 0)
                    media = 0;
            imagenAux.qimage.setPixel(u, v, media);
        }

    imagenAux.update();
    emit(changed());
}

void FDef::aceptar() {
    cambiarImagen();
    this->close();
    emit(acepted());
    //delete this;
}


void FDef::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    //delete this;
}


void FDef::quitarVentana() {
    if (firstPainted) {
        ui->verticalLayout->removeItem(&boxVentana);
        for (int i = 0; i < N; i++) { //atencion: N anterior
            for (int j = 0; j < N; j++)
                ventana[i][j].hide();

             boxVentana.removeItem(&boxFilas[i]);
        }
        /*delete [] boxFilas;
        for (int i = 0; i < N; i++)
            delete [] ventana[i];*/
    }

    //close();//show();
}

void FDef::initVentana() {
    //comprueba que sea tamaño impar y si no lo cambia
    N = ui->spinBoxTam->value(); //nuevo tamaño de ventana: NxN
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
            ventana[i][j].setRange(-20, 20);
            ventana[i][j].setValue(1); //valor por defecto (filtro de la media)
            boxFilas[i].addWidget(&ventana[i][j]); //añade a hbox la fila
        }

        boxVentana.addLayout(&boxFilas[i]); //añade hbox de fila en la vbox
    }

   ui->verticalLayout->addLayout(&boxVentana);
}



void FDef::crearVentana() {
    quitarVentana();
    initVentana();
}


void FDef::abrir() {
    //ifstream fin;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo de filtro"), QDir::currentPath());
   // if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
        return;
    }

    quitarVentana();

    char charAux;
    QTextStream in(&file);
    in >> N >> charAux; //N y \n  // cout << "\nN = " << N << flush;
    ui->spinBoxTam->setValue(N);
    initVentana(); //la crea con 1's

    double norm;
    in >> norm >> charAux; //valor normalizacion y \n
    ui->dSpinBoxNorm->setValue(norm);

    int value;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            in >> value >> charAux; //lee valor y espacio
            ventana[i][j].setValue(value);
        }
        in >> charAux; // lee \n
    }

}


void FDef::guardar() {
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo de filtro"), QDir::currentPath());
   // if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << N << "\n";
    out << ui->dSpinBoxNorm->value() << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << ventana[i][j].value() << " ";
        }
        out << "\n";
    }
}



