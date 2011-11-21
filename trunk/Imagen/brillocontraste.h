#ifndef BRILLOCONTRASTE_H
#define BRILLOCONTRASTE_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class BrilloContraste;
}

class BrilloContraste : public QWidget
{
    Q_OBJECT

public:
    explicit BrilloContraste(QWidget *parent = 0);
    ~BrilloContraste();



public slots:
    void setValues(Imagen * imagen);
    void AplicarCambios();

private:
    Ui::BrilloContraste *ui;
    //int Brillo;
    //int Contraste;

signals:
    void updateSliderB(int);
    void updateSpinBoxB(double);

    void updateSliderC(int);
    void updateSpinBoxC(double);

private slots:
    void intToDoubleB(int value);
    void DoubleToIntB(double value);
    void intToDoubleC(int value);
    void DoubleToIntC(double value);

};

#endif // BRILLOCONTRASTE_H
