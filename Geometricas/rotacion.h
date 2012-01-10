#ifndef ROTACION_H
#define ROTACION_H

#include <QWidget>
#include "imagen.h"

#define VMP 0  //indices de ui->comboxBoxInterpolacion
#define BILINEAL 1

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

#endif // ROTACION_H
