#include "function.h"
#include "ui_function.h"

Function::Function(QWidget *parent, QVector<double> func, double x_min, double x_max, double y_min, double y_max) :
    QWidget(parent)
    //ui(new Ui::Function)
{
    //ui->setupUi(this);

    setMinimumSize(256, 256);
    resize(500, 256);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    fun = func;
    this->xmin = x_min;
    this->xmax = x_max;
    this->ymin = y_min;
    this->ymax = y_max;

}

Function::~Function()
{
    //delete ui;
}


double Function::getX() {
    return xMouse;
}

double Function::getY() {
    return yMouse;
}

void Function::paintEvent(QPaintEvent * /* event */) {

    QPainter painter(this);
    QPen pen = QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); //    pen.setBrush(Qt::blue);

    painter.setPen(pen);
    painter.scale(1.0, -1.0); //invierte y
    painter.translate(0.0, (double) -height()); //el (0, 0) esta abajo como solemos ver en funciones

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
    //printf("\n(%d, %d)", posMouse.x(), posMouse.y());
}



