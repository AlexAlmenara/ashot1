#ifndef ASHOT_H
#define ASHOT_H

#include <QMainWindow>
#include <QPrinter>
#include <QtGui>

#include "Imagen/brillocontraste.h"
#include "Ayuda/ayuda.h"
#include "Ayuda/acercade.h"

namespace Ui {
    class aShot;
}

class aShot : public QMainWindow
{
    Q_OBJECT

public:
    explicit aShot(QWidget *parent = 0);
    ~aShot();

private:
    Ui::aShot *ui;
    BrilloContraste * bc;
    Ayuda * ayuda;
    Acercade * acercade;
    aShot * a;
    QImage * image;
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
    //void about();
    void prueba();


};

#endif // ASHOT_H
