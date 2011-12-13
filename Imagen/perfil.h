#ifndef PERFIL_H
#define PERFIL_H

#include <QWidget>
#include "imagen.h"

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

    QPen pen;
    QBrush brush;

    Imagen imagen;


protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PERFIL_H
