#include "perfil.h"
#include "ui_perfil.h"

Perfil::Perfil(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Perfil)
{
    ui->setupUi(this);

    setMinimumSize(256, 256);
    //resize(256, 256);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->imagen = imagen;
}

Perfil::~Perfil()
{
    delete ui;
}


void Perfil::paintEvent(QPaintEvent * /* event */) {

    QPainter painter(this);
    pen = QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); //    pen.setBrush(Qt::blue);

    painter.setPen(pen);
    painter.scale(1.0, -1.0); //invierte y
    painter.translate(0.0, (double) -height()); //el (0, 0) esta abajo como solemos ver en funciones


    //esto ya no hace falta, porque las coordenadas se eligen desde aShot
    /*int x1 = 5, y1 = 10; //coordenadas que introduce el usuario
    int x2 = 100, y2 = 200;

    int i1 = imagen.width() * (y1 - 1) + x1;
    int i2 = imagen.width() * (y2 - 1) + x2;*/

    double xdist = (double) this->width() / (double) (imagen.size()); //(i2 - i1); //incremento de x //cuidado con conversiones
    double ydist = (double) this->height() / (double) imagen.M(); //incremento de y
    double x; //para paint
    double y;
    for (int i = 0; i < imagen.size(); i++) { //desde i1 hasta i2
        x = i * xdist;
        //printf("i antes");
        y = imagen.dperfil(i) * ydist;
        //printf("i despues");
        painter.drawLine(x, 0.0, x, y);
    }
}
