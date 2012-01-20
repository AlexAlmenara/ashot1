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

    firstPainted = false;
    perfil();
    firstPainted = true;
}

Perfil::~Perfil()
{
    delete ui;
}


void Perfil::updatePunto() {
    QPoint p = imagen.posPerfil(function->getX());
    ui->labelPunto->setText("Pixel: " + QString::number((int) function->getX()) + " (" + QString::number(p.x()) + ", " + QString::number(p.y()) + "), Valor: " + QString::number(function->getY()));
}


void Perfil::newFunction(double ymax, int modo) {
    if (firstPainted) {
        function->hide();
        ui->hLayoutFunction->removeWidget(function);
    }

    if (modo == Y_MODE)
        function = new Function(this, hist, 0, imagen.nDiagonal() - 1, -ymax, ymax, modo); //para derivadas
    else
        function = new Function(this, hist, 0, imagen.nDiagonal() - 1, 0, ymax, modo); //para perfiles

    function->show();
    ui->hLayoutFunction->addWidget(function);
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

    newFunction((double) imagen.M(), Y_MODE);
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

    newFunction((double) imagen.M(), Y_MODE);
}





