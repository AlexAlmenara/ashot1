#include "diferencia.h"
#include "ui_diferencia.h"

Diferencia::Diferencia(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::Diferencia)
{
    ui->setupUi(this);

    move(600, 0);
    imagenOriginal = image;
    imagenAux = image;
    imagenEsp = Imagen();

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));
    connect(ui->ButtonAbrir, SIGNAL(clicked()), this, SLOT(abrir()));

    connect(ui->checkBoxhResult, SIGNAL(pressed()), this, SLOT(histResult()));
    connect(ui->checkBoxhSelect, SIGNAL(pressed()), this, SLOT(histEspecif()));

    connect(ui->checkBoxCambios1, SIGNAL(pressed()), this, SLOT(i1_cambios()));
    connect(ui->checkBoxCambios2, SIGNAL(pressed()), this, SLOT(i2_cambios()));

    ui->doubleSpinBoxN->setRange(0.0, (double) imagenOriginal.M() / 2.0);
    ui->doubleSpinBoxN->setValue(1.0);

    abrir();
}

Diferencia::~Diferencia()
{
    delete ui;
}


int Diferencia::dif(int x, int y) {
    return abs(imagenOriginal.gray(x, y) - imagenEsp.gray(x, y));
}

void Diferencia::updateLabel() {
    //label.hide();
    label.setPixmap(QPixmap::fromImage(imagenEsp.qimage));
    //label.resize(imagenEsp.width(), imagenEsp.height()); //se supone que abrimos siempre del tamaño de la imagen de trabajo
    label.move(this->x() + 600, this->y());
    label.show();
}



void Diferencia::aceptar() {
    this->close();
    emit(changed()); //no hace falta, ya lo hace abrir
    emit(acepted());
    delete this;
}


void Diferencia::cancelar() {
    imagenAux = imagenOriginal;
    close();
    emit(changed());
    delete this;
}


void Diferencia::abrir() { //reinicia, abre otra imagen y ejecuta la conversion
    imagenAux = imagenOriginal;

    //abrir imagen
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        imagenEsp = Imagen(fileName);
        if (imagenEsp.isNull()) {
            QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
            return;
        }

        updateLabel();
        //transformacion de imagen de trabajo
        for (int i = 0; i < imagenAux.width(); i++) // igual: qimage.width()
            for (int j = 0; j < imagenAux.height(); j++)
                imagenAux.qimage.setPixel(i, j, dif(i, j));

        imagenAux.update();
        //emit(changed());
    } //if

    emit(changed());
}


void Diferencia::histResult() { //histograma de la imagen diferencia
    Histograma * histograma;
    if (imagenAux.max_vin() == 0) //casi todo lo restado da 0: bajar escala en eje y
        histograma = new Histograma(0, imagenAux, (double) imagenAux.maxh() / 10.0);
    else
        histograma = new Histograma(0, imagenAux);
    histograma->show();
    histograma->move(600, 500);
}


void Diferencia::histEspecif() {
    Histograma * histograma = new Histograma(0, imagenEsp);
    histograma->show();
    histograma->move(1100, 500);
}



void Diferencia::i1_cambios() {
    Imagen i1 = imagenOriginal;

    int min = (int) (imagenAux.brillo() - ui->doubleSpinBoxN->value() * imagenAux.contraste()); //rango de umbral de decision
    int max = (int) (imagenAux.brillo() + ui->doubleSpinBoxN->value() * imagenAux.contraste()); //los pixeles de cambio de I1 o I2 son los q estan fuera de este rango de img Diferencia

    if (min < 0) min = 0;
    if (max > imagenAux.M() - 1) max = imagenAux.M() - 1;
    //printf("\nrango: [%d, %d]", min, max);

    int CAMBIO = 255; //valor que usaremos para dibujar con color verde
    if (min > 0) CAMBIO = 0; //if (max < 255) CAMBIO = 255;
    i1.qimage.setColor(CAMBIO, qRgb(0, 255, 0));

    for (int i = 0; i < i1.width(); i++)
        for (int j = 0; j < i1.height(); j++)
            if ((i1.gray(i, j) >= min) && (i1.gray(i, j) <= max)) //si este nivel de gris esta EN el rango no se marca (se pone un valor 0)
                i1.qimage.setPixel(i, j, CAMBIO); //los que estan dentro se pone 0, los que estan fuera (los de cambio) se dejan igual para mostrarlos
        i1.update();

    label1.setPixmap(QPixmap::fromImage(i1.qimage));
    label1.show();
}



void Diferencia::i2_cambios() {
    Imagen i2 = imagenEsp;

    int min = (int) (imagenAux.brillo() - ui->doubleSpinBoxN->value() * imagenAux.contraste()); //rango de umbral de decision
    int max = (int) (imagenAux.brillo() + ui->doubleSpinBoxN->value() * imagenAux.contraste()); //los pixeles de cambio de I1 o I2 son los q estan fuera de este rango de img Diferencia

    if (min < 0) min = 0;
    if (max > imagenAux.M() - 1) max = imagenAux.M() - 1;

    int CAMBIO = 255; //valor que usaremos para dibujar con color verde
    if (min > 0) CAMBIO = 0; //if (max < 255) CAMBIO = 255;
    i2.qimage.setColor(CAMBIO, qRgb(0, 255, 0));

    for (int i = 0; i < i2.width(); i++)
        for (int j = 0; j < i2.height(); j++)
            if ((i2.gray(i, j) >= min) && (i2.gray(i, j) <= max)) //si este nivel de gris esta EN el rango no se marca (se pone un valor 0)
                i2.qimage.setPixel(i, j, 255); //los que estan dentro se pone 0, los que estan fuera (los de cambio) se dejan igual para mostrarlos
        i2.update();

    label2.setPixmap(QPixmap::fromImage(i2.qimage));
    label2.show();
}
