#ifndef IMAGEN_H
#define IMAGEN_H

#include <QString>
#include <QImage>
#include <QtGui>

#include <QMainWindow>
#include <QPrinter>

class Imagen {

private:
    int * hist;
    int Mniveles; //numero total de niveles
    QString file;
    int err; //error: imagen null, o se usa un formato que no hemos albergado para vect
    void createHist();

public:
    Imagen();
    Imagen(QString fileName);

    QImage * qimage;

    int error(); //se saca el error para que lo lea aShot y sace un MessageBox
    int M(); //numero total de niveles
    int max();
    int min();
    double brillo();
    double contraste();
    double entropia();

    double rel(int i); //frecuencia relativa del nivel i
    int height();
    int width();
    int size();

    void update();
    //int pixel(int x, int y);
    //void setPixel(int value);

    QImage qImage();
    QString fileName();
    QString extension();
    QString formato();
    bool isNull();


};

#endif // IMAGEN_H
