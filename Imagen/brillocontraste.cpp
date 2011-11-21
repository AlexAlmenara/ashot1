#include "brillocontraste.h"
#include "ui_brillocontraste.h"

BrilloContraste::BrilloContraste(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrilloContraste)
{
    ui->setupUi(this);
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(AplicarCambios()));
    //connect(ui->ButtonReiniciar, SIGNAL(clicked()), this, SLOT(setV));

    //setValues(); //por eficiencia, no lo ponemos aqui, se hace antes de hacer el show()

    //brillo
    connect(ui->SliderBrillo, SIGNAL(valueChanged(int)), this, SLOT(intToDoubleB(int)));
    connect(this, SIGNAL(updateSpinBoxB(double)), ui->doubleSpinBoxBrillo, SLOT(setValue(double)));

    connect(ui->doubleSpinBoxBrillo, SIGNAL(valueChanged(double)), this, SLOT(DoubleToIntB(double)));
    connect(this, SIGNAL(updateSliderB(int)), ui->SliderBrillo, SLOT(setValue(int)));

    //contraste

    connect(ui->SliderContraste, SIGNAL(valueChanged(int)), this, SLOT(intToDoubleC(int)));
    connect(this, SIGNAL(updateSpinBoxC(double)), ui->doubleSpinBoxContraste, SLOT(setValue(double)));

    connect(ui->doubleSpinBoxContraste, SIGNAL(valueChanged(double)), this, SLOT(DoubleToIntC(double)));
    connect(this, SIGNAL(updateSliderC(int)), ui->SliderContraste, SLOT(setValue(int)));
}





BrilloContraste::~BrilloContraste() {
    delete ui;
}


void BrilloContraste::setValues(Imagen * imagen) {
    //brillo
    ui->SliderBrillo->setRange(0, imagen->M() - 1);
    ui->SliderBrillo->setValue(imagen->brillo());
    ui->doubleSpinBoxBrillo->setRange(0.0, (double) imagen->M() - 1);
    ui->doubleSpinBoxBrillo->setValue(imagen->brillo());

    //contraste
    ui->SliderContraste->setRange(0, imagen->M() - 1); //???
    ui->SliderContraste->setValue(imagen->contraste());
    ui->doubleSpinBoxContraste->setRange(0.0, (double) imagen->M() - 1);
    ui->doubleSpinBoxContraste->setValue(imagen->contraste());
}



//slots:

void BrilloContraste::intToDoubleB(int value) {
    double convertedValue = (double) value;
    emit(updateSpinBoxB(convertedValue)); //se emite signal
}

void BrilloContraste::DoubleToIntB(double value) {
    int convertedValue = (int) value;
    emit(updateSliderB(convertedValue)); //se emite signal
}


void BrilloContraste::intToDoubleC(int value) {
    double convertedValue = (double) value;
    emit(updateSpinBoxC(convertedValue)); //se emite signal
}

void BrilloContraste::DoubleToIntC(double value) {
    int convertedValue = (int) value;
    emit(updateSliderC(convertedValue)); //se emite signal
}


void BrilloContraste::AplicarCambios() {
    //se aplican los cambios
    this->close();
}
