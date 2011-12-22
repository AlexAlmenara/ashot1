#include "perfil.h"
#include "ui_perfil.h"

Perfil::Perfil(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Perfil)
{
    ui->setupUi(this);


    this->imagen = imagen;
    setWindowTitle("Perfil de " + imagen.fileName());

    connect(ui->radioButtonPerfil, SIGNAL(clicked()), this, SLOT(perfil()));
    connect(ui->radioButtondPerfil, SIGNAL(clicked()), this, SLOT(dperfil()));
    connect(ui->radioButtonPerfilSuave, SIGNAL(clicked()), this, SLOT(perfilSuave()));
    connect(ui->radioButtondPerfilSuave, SIGNAL(clicked()), this, SLOT(dperfilSuave()));
    ui->radioButtonPerfil->setChecked(true); //empieza por defecto perfil normal
    perfil();

}

Perfil::~Perfil()
{
    delete ui;
}


void Perfil::updatePunto() {
    QPoint p = imagen.posPerfil(function->getX());
    ui->labelPunto->setText("Pixel: " + QString::number((int) function->getX()) + " (" + QString::number(p.x()) + ", " + QString::number(p.y()) + "), Valor: " + QString::number(function->getY()));
}


void Perfil::newFunction(double ymax) {
    function = new Function(this, hist, 0, imagen.nDiagonal() - 1, 0, ymax); //por defecto histograma absoluto
    function->resize(400, 200);
    function->show();
    function->move(40, 20);
    connect(function, SIGNAL(moused()), this, SLOT(updatePunto()));
}


void Perfil::perfil() {
    hist = QVector<double>(imagen.nDiagonal());
    //printf("nDiagonal: %d\n", imagen.nDiagonal());
    for (int i = 0; i < imagen.nDiagonal(); i++) //recorre la diagonal
        hist.insert(i, imagen.perfil(i));

    newFunction((double) imagen.M());
}

void Perfil::dperfil() {
    hist = QVector<double>(imagen.nDiagonal());
    for (int i = 0; i < imagen.nDiagonal(); i++) //recorre la diagonal
        hist.insert(i, imagen.dperfil(i));

    newFunction((double) imagen.M());
}


void Perfil::perfilSuave() {
    hist = QVector<double>(imagen.nDiagonal());
    for (int i = 0; i < imagen.nDiagonal(); i++) //recorre la diagonal
        hist.insert(i, imagen.perfilSuave(i));

    newFunction((double) imagen.M());
}


void Perfil::dperfilSuave() {
    hist = QVector<double>(imagen.nDiagonal());
    for (int i = 0; i < imagen.nDiagonal(); i++) //recorre la diagonal
        hist.insert(i, imagen.dperfilSuave(i));

    newFunction((double) imagen.M());
}





