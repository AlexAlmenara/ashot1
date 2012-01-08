#ifndef PERFIL_H
#define PERFIL_H

#include <QWidget>
#include "imagen.h"
#include "function.h"

namespace Ui {
    class Perfil;
}

class Perfil : public QWidget
{
    Q_OBJECT

public:
    explicit Perfil(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Perfil();

private:
    Ui::Perfil *ui;

    QVector<double> hist;
    Function * function;
    Imagen imagen;

    void newFunction(double ymax, int modo = SIMPLE_MODE);


public slots:
    void updatePunto();
    void perfil();
    void dperfil();
    void perfilSuave();
    void dperfilSuave();
};

#endif // PERFIL_H
