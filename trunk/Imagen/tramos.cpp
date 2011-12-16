#include "tramos.h"
#include "ui_tramos.h"

Tramos::Tramos(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Tramos)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->ButtonNtramos, SIGNAL(clicked()), this, SLOT(introNtramos()));
    connect(ui->ButtonPunto, SIGNAL(clicked()), this, SLOT(introPuntoi()));

    ui->spinBox_nTramos->setRange(1, 20); //maximo 20 tramos
    ui->spinBox_x->setRange(0, imagenAux.M() - 1); //rangos
    ui->spinBox_y->setRange(0, 900); //imagenAux.size());

    ui->spinBox_nTramos->setValue(1); //valores por defecto
    ui->spinBox_x->setValue(0);
    ui->spinBox_x->setReadOnly(true); //el primer punto debe ser x = 0
    ui->spinBox_y->setValue(20);
    ui->label_punto->setText("Punto 0 (x, y): ");

    imagenAux =  imagen;
    nTramos = 1;
    puntoi = 0;
    //puntos = new QPoint [1];

    ui->ButtonPunto->setEnabled(false); //se activara cuando se introduzca numero de tramos
    ui->ButtonAceptar->setEnabled(false);
}

Tramos::~Tramos()
{
    delete ui;
}


void Tramos::introNtramos() {
    nTramos = ui->spinBox_nTramos->value();
    puntos = new QPoint [nTramos + 1];

    ui->ButtonPunto->setEnabled(true);
    ui->ButtonNtramos->setEnabled(false);
}


void Tramos::introPuntoi() { //al menos se introduciran dos puntos.
    ui->spinBox_x->setReadOnly(false);

    puntos[puntoi].setX(ui->spinBox_x->value());
    puntos[puntoi].setY(ui->spinBox_y->value());

    if (puntoi == nTramos) { //0..nTramos, son (nTramos+1)
        ui->ButtonPunto->setEnabled(false); //ya no se introducen mas puntos
        ui->ButtonAceptar->setEnabled(true); //para terminar
        //Function * func = new Function(0, vector, 0.0, (double) imagenAux.M(), 0.0, (double) imagenAux.size() / (double) imagenAux.M());
        //func->show();
        return;
    }

    puntoi++;
    ui->label_punto->setText((const QString) "Punto " + QString::number(puntoi) + " (x, y): ");

    if (puntoi == nTramos) {
        ui->spinBox_x->setValue(imagenAux.M());
        ui->spinBox_x->setReadOnly(true); //el ultimo punto debe ser x = 255
    }
}


//slots:
void Tramos::aceptar() {
    int * vout = new int [imagenAux.M()]; //tabla LUT

    int tramo_i = 0; //tramo actual

    /*puntos[0].setX(0);
    puntos[0].setY(50);
    puntos[1].setX(86);
    puntos[1].setY(100);
    puntos[2].setX(172);
    puntos[2].setY(50);
    puntos[3].setX(255);
    puntos[3].setY(134);*/

    /*for (int i = 0; i <= nTramos; i++)
        printf("%d: (%d, %d)\n", i, puntos[i].x(), puntos[i].y());*/

    for (int vin = 0; vin < imagenAux.M(); vin++) {

        if ((vin > puntos[tramo_i + 1].x()) && (tramo_i < nTramos - 1) && (nTramos > 1)) //x es el limite i
            tramo_i++; //siguiente tramo

        //printf(", tramoi: %d", tramo_i);

        //vout = A*vin + B
        vout[vin] = puntos[tramo_i + 1].x() - puntos[tramo_i].x(); //max - min
        if (vout[vin] == 0) vout[vin] = 1;
        vout[vin] = (int) (((double) (puntos[tramo_i + 1].y() - puntos[tramo_i].y())) / ((double) (vout[vin]))); //(H-L) / (max -min)
        vout[vin] = puntos[tramo_i].y() + ((vin - puntos[tramo_i].x()) * vout[vin]); // L + (vin - min) * anterior
        //vout[vin] = 200;

        if (vout[vin] > imagenAux.M() - 1) //en caso de que se haya salido del rango de niveles (0..255)
            vout[vin] = imagenAux.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }

    imagenAux.transformar(vout); //transformacion. tambien hace el update de histograma

    this->close();
    //delete puntos;
    emit(closed()); //se emite signal
}



void Tramos::cancelar() {
    close();
    //delete puntos;
    emit(closed()); //se emite signal
}



/*
void aShot::prueba() {
    if (imagen.qimage.format() != 3) { //8-bit indexado, monocromo
        QMessageBox::information(this, tr("Informacion de la imagen"), "no es monocromo indexado");
        return;
    }

    //mas pruebas
    QRgb value;
    value = qRgb(122, 163, 39); // 0xff7aa327
    imagen.qimage.setColor(0, value);

    value = qRgb(237, 187, 51); // 0xffedba31
    imagen.qimage.setColor(1, value);

    //value = qRgb(189, 149, 39); // 0xffbd9527
    //image.setColor(2, value);

    for (int i = 0; i < imagen.width(); i++)
        for (int j = 0; j < imagen.height(); j++) {
            if (i == j)
                imagen.qimage.setPixel(i, j, 0);
            else
                imagen.qimage.setPixel(i, j, 1);
        }
    //fin mas pruebas


    int ntramos = 3;
    double * A = new double[ntramos];
    double * B = new double [ntramos];
    int * extremos = new int [ntramos]; //seria ntramos -1, pero se queda uno mas para q no se qede size 0 en caso de q ntramos=1
    int tramo_i = 0; //tramo actual
    //tambien: Qimage.setColorTable(QVector<QRgb>)
    int * vout = new int [imagen.M()]; //tabla LUT

    //intro datos
    A[0] = 3; B[0] = 2;
    A[1] = 3; B[1] = 0;
    A[2] = 0.3; B[2] = -1;
    extremos[0] = 20; //nivel 20
    extremos[1] = 180;
    //fin intro datos
    for (int vin = 0; vin < imagen.M(); vin++) {

        if (ntramos > 1)
            if (vin == extremos[tramo_i])
                tramo_i++; //siguiente tramo

        vout[vin] = (int) (A[tramo_i] * (double) vin + B[tramo_i]);
        if (vout[vin] > imagen.M() - 1) //en caso de que se haya salido del rango de niveles (0..255)
            vout[vin] = imagen.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }

     imagen.transformar(vout);
     updateImageLabel();  //actualiza label

 }*/
