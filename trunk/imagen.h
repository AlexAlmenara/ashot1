#ifndef IMAGEN_H
#define IMAGEN_H

#include <QString>
#include <QImage>
#include <QtGui>

#include <QMainWindow>
#include <QPrinter>

class Imagen {

private:
    int * hist; //histograma: frecuencias absolutas de los niveles. Si fuera COLOR: 3 hist
    int * acum; //histograma acumulado: frec acumuladas

    int Mniveles; //numero total de niveles
    QString file;
    int err; //error: imagen null, o se usa un formato que no hemos albergado para vect
    void createHist();

public:
    Imagen();
    Imagen(QString fileName);

    QImage qimage;

    int error(); //se saca el error para que lo lea aShot y sace un MessageBox
    int M(); //numero total de niveles
    int max();
    int min();
    double brillo();
    double contraste();
    double entropia();

    int hAbs(int i); //frecuencia absoluta del nivel i
    double hRel(int i); //frecuencia relativa del nivel i
    int hAcum(int i); //frecuencia acumulada del nivel i
    double hAcumNorm(int i); //frecuencia acumulada NORMALIZADO del nivel i

    int height();
    int width();
    int size();

    void update(); //actualiza info asociada a imagen: vector hist
    void transformar(int * vout); //trasnforma una imagen con una tabla LUT dada, es decir, vector vout
    //int pixel(int x, int y);
    //void setPixel(int value);

    QImage qImage();
    QString fileName();
    QString extension();
    QString formato();
    bool isNull();


};

#endif // IMAGEN_H
