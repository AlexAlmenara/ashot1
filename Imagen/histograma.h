#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <QWidget>
#include "imagen.h"
#include "function.h"

namespace Ui {
    class Histograma;
}

class Histograma : public QWidget
{
    Q_OBJECT

public:
    explicit Histograma(QWidget *parent = 0, Imagen image = Imagen());
    ~Histograma();

private:
    Ui::Histograma *ui;

    QVector<double> hist;
    Function * function;
    Imagen imagen;

    //int maxh; //por si el usuario quiere especificar una escala diferente en y (puede pasar que no se vea bien
    //bool firstPainted;

    void newFunction(double ymax);


public slots:
    void updatePunto();
    void histAbs();
    void histRel();
    void histAcum();
    void histAcumNorm();

//protected:
  //  void mouseMoveEvent(QMouseEvent *);
    //void paintEvent(QPaintEvent *event);
};

#endif // HISTOGRAMA_H
