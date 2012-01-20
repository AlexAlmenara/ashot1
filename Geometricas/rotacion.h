#ifndef ROTACION_H
#define ROTACION_H

#include <QWidget>
#include "imagen.h"
#include <iostream>
#include <stdio.h>

using namespace std;

#define PI 3.14159
#define R90 1.57079
#define R180 PI
#define R270 4.71238
#define R360 6.28318

#define VMP 0  //indices de ui->comboxBoxInterpolacion
#define BILINEAL 1


#define DIRECTA 0 //indices de ui->comboBoxTransf
#define INDIRECTA 1


//clase de punto (x, y) de double
class FPoint {
    double x;
    double y;

public:
    FPoint() { x = 0.0; y = 0.0; }
    FPoint(double xin, double yin) { x = xin; y = yin; }

    void setValues(double xin, double yin) { x = xin; y = yin; }
    void setX(double xin) { x  = xin; }
    void setY(double yin) { y  = yin; }

    double getX() { return x; }
    double getY() { return y; }
};

ostream &operator << (ostream &sout, FPoint &p);


double toRad(double deg); //pasa angulo de Deg a Rad

namespace Ui {
    class Rotacion;
}

class Rotacion : public QWidget
{
    Q_OBJECT

public:
    explicit Rotacion(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Rotacion();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::Rotacion *ui;

    Imagen imagenOrig;
    double ang; //angulo de rotacion (- ó +)
    int anchura, altura; //nuevas dimensiones

    FPoint Ep, Fp, Gp, Hp; //4 esquinas

    FPoint firstTD(int x, int y); //transformacion directa, pero aun sin cambiar los ejes de coordenadas
    FPoint tD(int x, int y); //transformacion directa: (x, y) de imagenOrig, devuelve (xp, yp) de imagen rotada (llama a fistTD)

    FPoint firstTI(int xp, int yp); //transformacion indirecta, pero aun sin cambiar los ejes de coordenadas
    FPoint tI(int xp, int yp); //transformacion indirecta: (xp, yp) de imagen rotada, devuelve (x, y) de imagenOrig (llama a firstTI)

    void initRotacion(); //inicializa imagen rotada: calcula 4 esquinas y paralelogramo

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();

signals:
    void changed();
    void acepted();

    void updateSlider(int);
    void updateSpinBox(double);

private slots:
    void intToDouble(int value);
    void DoubleToInt(double value);
};

#endif // ROTACION_H
