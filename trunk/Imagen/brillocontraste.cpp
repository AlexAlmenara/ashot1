#include "brillocontraste.h"
#include "ui_brillocontraste.h"

BrilloContraste::BrilloContraste(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrilloContraste)
{
    ui->setupUi(this);
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(AplicarCambios()));
}


BrilloContraste::~BrilloContraste()
{
    delete ui;
}


void BrilloContraste::AplicarCambios() {
    //se aplican los cambios
    this->close();
}
