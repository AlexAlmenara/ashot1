#include "function.h"
#include "ui_function.h"

#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QtGui>
#include <stdio.h>

Function::Function(QWidget *parent, QVector<double> func, double x_min, double x_max, double y_min, double y_max, int mode) :
    QWidget(parent)
{
    setMinimumSize(256, 256);
    resize(500, 256);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    fun = func;
    this->xmin = x_min;
    this->xmax = x_max;
    this->ymin = y_min;
    this->ymax = y_max;
    modo = mode;
}

Function::~Function()
{ }


//atencion: para terminar de generalizar falta poner casos X_MODE y TOTAL_MODE

double Function::getX() {
    return xMouse;
}

double Function::getY() {
    return yMouse;
}

void Function::paintEvent(QPaintEvent * /* event */) {

    QPainter painter(this);
    painter.scale(1.0, -1.0); //invierte y

    switch (modo) {
    case SIMPLE_MODE:
        painter.translate(0.0, (double) -height()); //el (0, 0) esta abajo como solemos ver en funciones
        break;

    case Y_MODE:
        painter.translate(0.0, (double) (-height() / 2.0)); // (0, 0) en medio
        painter.drawLine(0.0, 0.0, width(), 0.0);
        break;
    }

    QPen pen = QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); //    pen.setBrush(Qt::blue);
    painter.setPen(pen);

    double xdist = (double) this->width() / (xmax - xmin); //incremento de x //cuidado con conversiones
    double ydist = (double) this->height() / (ymax - ymin); //incremento de y
    double x;
    double y;

    for (int vin = xmin; vin <= xmax; vin++) {
        x = vin * xdist;
        y = fun.at(vin - xmin) * ydist; //cuidado, no se puede salir del vector, se pondria un if pero es ineficiente
        painter.drawLine(x, 0.0, x, y);
    }

}


//protected: events
/*void Function::mousePressEvent(QMouseEvent* event) {
    double xdist = (double) this->width() / (double) (xmax - xmin); //incremento de x //cuidado con conversiones
    int vin = (int) ((double) event->x() / xdist);
    posMouse = QPoint(vin + xmin, fun.at(vin));
    emit(moused());
    //printf("\n(%d, %d)", posMouse.x(), posMouse.y());
    //ui->statusBar->showMessage((const QString) "(" + QString::number(x) + ", " + QString::number(y) + ") = " + QString::number(imagen.perfil(x, y)));

}*/

void Function::mouseMoveEvent(QMouseEvent* event) {
    double xdist = (double) this->width() / (xmax - xmin); //incremento de x //cuidado con conversiones
    int vin = (int) ((double) event->x() / xdist);
    if ((vin >= xmin) && (vin <= xmax) && (fun.at(vin) >= ymin) && (fun.at(vin) <= ymax)) {
        xMouse = vin + xmin;
        yMouse = fun.at(vin);
        emit(moused());
    }
}



