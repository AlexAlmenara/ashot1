#ifndef ASHOT_H
#define ASHOT_H

#include <QMainWindow>
#include <QPrinter>
#include <QtGui>

#include "Imagen/brillocontraste.h"
#include "Ayuda/ayuda.h"
#include "imagen.h"

namespace Ui {
    class aShot;
}

class aShot : public QMainWindow
{
    Q_OBJECT

public:
    explicit aShot(QWidget *parent = 0);
    ~aShot();
    void updateImageLabel();

    Imagen * imagen;

private:
    Ui::aShot *ui;
    BrilloContraste * bc;
    Ayuda * ayuda;
    aShot * a;

    double scaleFactor;
    //bool hasImage;


#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    void connectActions();
    void enableActions();
    void updateZoomActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);


private slots:
    void createNewWindow();
    void abrir();
    void cerrarTodo();
    void imprimir();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void info_imagen();
    void acercade();
    void guardar();
    void guardarComo();
    void toGray(); //convierte imagen a escala de grises
    void negativo(); //convierte imagen a negativo

    //para sacar widgets
    void showNewBC(); //crea bc y lo muestra
    void aplicarBC(); //aplica los cambios hechos despues de cerrar el widget BrilloContraste

    void prueba();
    void prueba2();
    void prueba3();


};

#endif // ASHOT_H
