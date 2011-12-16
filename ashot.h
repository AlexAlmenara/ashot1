#ifndef ASHOT_H
#define ASHOT_H

#include <QMainWindow>
#include <QPrinter>
#include <QtGui>

#include "Ayuda/ayuda.h"
#include "imagen.h"
#include "Imagen/brillocontraste.h"
#include "Imagen/logexp.h"
#include "Imagen/histograma.h"
#include "Imagen/perfil.h"
#include "Imagen/tramos.h"
#include "Imagen/hespecif.h"
#include "Imagen/digitalizar.h"

namespace Ui {
    class aShot;
}

class aShot : public QMainWindow
{
    Q_OBJECT

public:
    explicit aShot(QWidget *parent = 0);
    ~aShot();

    void abrir();
    void updateImageLabel();
    void updateAll(); //actualiza imagen y imageLabel

    Imagen imagen; //imagen entera que se usa
    Imagen imagenRect; //imagen de region seleccionada (si no hay region es igual a imagen), esta es realmente con la que se trabaja

private:
    Ui::aShot *ui;
    BrilloContraste * bc;
    Ayuda * ayuda;
    Logexp * logexp;
    Histograma * histograma;
    Perfil * perfil;
    Tramos * tramos;
    Hespecif * hespecif;
    Digitalizar * digit; //digitalizar imagen: se aplica a la imagen entera (los demas widgets lo hacen en imagenRect)

    aShot * a;

    double scaleFactor;
    bool hasImage;

    QPoint p1, p2; //puntos para region de seleccion (QRect rect)


#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    void connectActions();
    void enableActions();
    void updateZoomActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void pegarImagenRect(); //pega la imagenRect en imagen


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *); //CUIDADO: si la imagen no cabe aun no lo hace bien
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    //void mouseGrabber();

private slots:
    void abrirNew();
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
    void toGray(); //convierte imagen a escala de grises. DOS COSAS: una para convertir formato de imagen, y otra blanco/negro de imagenRect
    void negativo(); //convierte imagen a negativo

    //para sacar widgets
    void showNewBC(); //crea bc y lo muestra
    void applyBC(); //aplica los cambios hechos despues de cerrar el widget BrilloContraste
    void showNewLogexp();
    void applyLogexp();
    void showNewTramos();
    void applyDestroyTramos();
    void showNewHespecif();
    void applyHespecif();
    void showNewDigit();
    void applyDigit();

    void showNewHistograma();
    void showNewPerfil();

    void prueba();
    void ecualizar();


};

#endif // ASHOT_H
