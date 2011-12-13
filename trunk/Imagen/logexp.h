#ifndef LOGEXP_H
#define LOGEXP_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class Logexp;
}

class Logexp : public QWidget
{
    Q_OBJECT

public:
    explicit Logexp(QWidget *parent = 0, Imagen imagen = Imagen(), QLabel * label = NULL);
    ~Logexp();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios
    QLabel * imageLabel;

private:
    Ui::Logexp *ui;

    //funciones de transformacion: vout[vin] = f(vin, k)
    int log1(int vin);
    int log2(int vin, int k);
    int log3(int vin, int k);

    int exp1(int vin);
    int exp2(int vin, int k);
    int exp3(int vin, int k);


signals:
    void closed();

public slots:
    void aplicarCambios();
    void cancelar();


};

#endif // LOGEXP_H
