#include "histograma.h"
#include "ui_histograma.h"

Histograma::Histograma(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Histograma)
{
    ui->setupUi(this);

    setMinimumSize(256, 256);
    //resize(256, 256);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->imagen = imagen;
}

Histograma::~Histograma()
{
    delete ui;
}


void Histograma::paintEvent(QPaintEvent * /* event */) {

    QPainter painter(this);
    pen = QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    painter.setPen(pen);
    painter.scale(1.0, -1.0); //invierte y
    painter.translate(0.0, (double) -height()); //el (0, 0) esta abajo como solemos ver en funciones

    pen.setBrush(Qt::blue);
    painter.setPen(pen);

    double xdist = (double) this->width() / (double) imagen.M(); //incremento de x //cuidado con conversiones
    double ydist = (double) this->height() / (double) imagen.maxh(); //incremento de y
    double x;
    double y;
    for (int vin = 0; vin < imagen.M(); vin++) {
        x = vin * xdist;
        y = imagen.hAbs(vin) * ydist;
        painter.drawLine(x, 0.0, x, y);
    }
}
