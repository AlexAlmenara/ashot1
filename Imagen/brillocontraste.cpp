#include "brillocontraste.h"
#include "ui_brillocontraste.h"

BrilloContraste::BrilloContraste(QWidget *parent, Imagen imagen, QLabel * label) :
    QWidget(parent),
    ui(new Ui::BrilloContraste)
{
    ui->setupUi(this);
    //shot = ashot; //se igualan los punteros, se tiene un solo objeto con dos punteros apuntandole

    //connect(this, SIGNAL(destroyed()), this, SLOT(cancelar()));
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aplicarCambios()));
    connect(ui->ButtonReiniciar, SIGNAL(clicked()), this, SLOT(reiniciar()));


    setValues(imagen, label); //por eficiencia, no lo ponemos aqui, se hace antes de hacer el show()


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


    //cambios de slider y spinbox cambian imageLabel directamente
    connect(ui->doubleSpinBoxBrillo, SIGNAL(valueChanged(double)), this, SLOT(changeImageLabel()));
    connect(ui->doubleSpinBoxContraste, SIGNAL(valueChanged(double)), this, SLOT(changeImageLabel()));

}





BrilloContraste::~BrilloContraste() {
    delete ui;
}


void BrilloContraste::setValues(Imagen imagen, QLabel * label) {
    //brillo
    ui->SliderBrillo->setRange(0, imagen.M() - 1);
    ui->SliderBrillo->setValue(imagen.brillo());
    ui->doubleSpinBoxBrillo->setRange(0.0, (double) imagen.M() - 1);
    ui->doubleSpinBoxBrillo->setValue(imagen.brillo());

    //contraste
    ui->SliderContraste->setRange(0, (int) ((double) imagen.M() / 2.0) );
    ui->SliderContraste->setValue(imagen.contraste());
    ui->doubleSpinBoxContraste->setRange(0.0, ((double) imagen.M() / 2.0) );
    ui->doubleSpinBoxContraste->setValue(imagen.contraste());

    imagenOriginal = imagen; //Imagen(imagen.fileName()); //se igualan los punteros
    imagenAux =  imagen; //new Imagen(imagen->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    imageLabel = label; //se igualan los punteros: ambos apuntan a la misma label donde se pinta la imagen
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


//cambios de imagen y label
void BrilloContraste::changeImageLabel() {
    imagenAux =  imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.

    double brillo2 = ui->doubleSpinBoxBrillo->value();
    double contraste2 = ui->doubleSpinBoxContraste->value(); //tambien: Qimage.setColorTable(QVector<QRgb>)
    double A = contraste2 / imagenAux.contraste();
    double B = brillo2 - A * imagenAux.brillo();

    int * vout = new int [imagenAux.M()]; //tabla LUT
    for (int vin = 0; vin < imagenAux.M(); vin++) {
        vout[vin] = (int) (A * (double) vin + B);
        if (vout[vin] > imagenAux.M() - 1)
            vout[vin] = imagenAux.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }


    imagenAux.transformar(vout); //transformacion. tambien hace el update de histograma
    imageLabel->setPixmap(QPixmap::fromImage(imagenAux.qimage)); //shot->updateImageLabel();//actualiza label
}



/*void BrilloContraste::updateShow() {
    setValues(shot->imagen);
    show();
}*/


void BrilloContraste::aplicarCambios() {
    //se aplican los cambios, ya no hace falta actualizar label
    //delete imagenOriginal; //destruye
    //imagenOriginal = NULL;
    //imagenOriginal = new Imagen(imagenAux->fileName()); //no hace falta, porque esto ya se hace desde aShot
    //imagenOriginal = imagenAux;
    this->close();
    emit(closed()); //se emite signal
}


void BrilloContraste::cancelar() {
    imageLabel->setPixmap(QPixmap::fromImage(imagenOriginal.qimage)); //shot->updateImageLabel();//actualiza label
    imagenAux =  imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    close();
    emit(closed()); //se emite signal
    //delete this;
}


void BrilloContraste::reiniciar() {
    imageLabel->setPixmap(QPixmap::fromImage(imagenOriginal.qimage)); //shot->updateImageLabel();//actualiza label

    //ui->SliderBrillo->setValue(imagenOriginal->brillo()); //no hace falta, ya se igualan a los spinBox
    //ui->SliderContraste->setValue(imagenOriginal->brillo());
    ui->doubleSpinBoxBrillo->setValue(imagenOriginal.brillo());
    ui->doubleSpinBoxContraste->setValue(imagenOriginal.contraste());
    imagenAux = imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
}



