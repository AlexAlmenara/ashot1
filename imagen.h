#ifndef IMAGEN_H
#define IMAGEN_H

#include <QString>
#include <QImage>
#include <QtGui>

#include <QMainWindow>
#include <QPrinter>

class Imagen {

private:
    QImage * image;
    int * vect;
    int Mniveles; //numero total de niveles
    QString file;
    int err; //error: imagen null, o se usa un formato que no hemos albergado para vect
    void createVect();

public:
    Imagen();
    Imagen(QString fileName);

    int error(); //se saca el error para que lo lea aShot y sace un MessageBox
    int M(); //numero total de niveles
    int max();
    int min();
    int brillo();
    int contraste();
    int entropia();

    int height();
    int width();
    int size();

    //int pixel(int x, int y);
    //void setPixel(int value);

    QString fileName();
    QString extension();
    QString formato();
    bool isNull();


};

#endif // IMAGEN_H
