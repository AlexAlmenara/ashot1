#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class Histograma;
}

class Histograma : public QWidget
{
    Q_OBJECT

public:
    explicit Histograma(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Histograma();

private:
    Ui::Histograma *ui;

    QPen pen;
    QBrush brush;

    Imagen imagen;


protected:
    void paintEvent(QPaintEvent *event);
};

#endif // HISTOGRAMA_H
