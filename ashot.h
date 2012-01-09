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
#include "Imagen/diferencia.h"

//Practica 2:
//ruidos
#include "Filtros/runiforme.h"
#include "Filtros/rimpulsivo.h"
#include "Filtros/rgaussiano.h"

//filtros
#include "Filtros/fmedia.h"
#include "Filtros/fmoda.h"
#include "Filtros/fmediana.h"
#include "Filtros/fdifest.h"
#include "Filtros/fvecinos.h"
#include "Filtros/fdef.h"
#include "Filtros/fgauss.h"


//herramientas
#define H_CURSOR 0
#define H_SELECCION 1
#define H_PINCEL 2




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
    Imagen imagen; //imagen entera que se usa
    Imagen imagenRect; //imagen de region seleccionada (si no hay region es igual a imagen), esta es realmente con la que se trabaja
    Imagen imagenAnt; //imagen anterior a cambio para volcarla luego en Deshacer

    QStack<Imagen> undo; //vectores para guardar ultimos cambios
    QStack<Imagen> redo;

    QLabel labelAnt; //label de la imagen anterior

    Ui::aShot *ui;
    BrilloContraste * bc;
    Logexp * logexp;
    Tramos * tramos;
    Hespecif * hespecif;
    Digitalizar * digit; //digitalizar imagen: se aplica a la imagen entera (los demas widgets lo hacen en imagenRect)
    Diferencia * diferencia;

    Ayuda * ayuda;
    //Histograma * histograma;
    //Perfil * perfil;

    aShot * a;

    double scaleFactor;
    bool hasImage;

    QPoint p1, p2; //puntos para region de seleccion (QRect rect)

    //Practica 2
    //ruidos
    RUniforme * rUniforme;
    RImpulsivo * rImpulsivo;
    RGaussiano * rGaussiano;

    //filtros
    FMedia * fMedia;
    FModa * fModa;
    FMediana * fMediana;
    FDifest * fDifest;
    FVecinos * fVecinos;
    FDef * fDef;
    FGauss * fGauss;


    //Practica 3 y mas
    int herramienta;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    void abrir();
    void updateImageLabel();
    void updateAll(); //actualiza imagen y imageLabel

    void connectActions();
    void enableActions(bool b);
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

public slots:
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

    void addDeshacer();
    void deshacer();
    void rehacer();
    void anteriorHistograma();
    void anteriorImagen();

    void prueba();
    void ecualizar();

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
    void showNewDiferencia();
    void applyDiferencia();

    void showNewHistograma();
    void showNewPerfil();

    //Practica 2
    //ruidos
    void showNewRUniforme();
    void applyRUniforme();
    void showNewRImpulsivo();
    void applyRImpulsivo();
    void showNewRGaussiano();
    void applyRGaussiano();

    //filtros
    void showNewFMedia();
    void applyFMedia();
    void showNewFModa();
    void applyFModa();
    void showNewFMediana();
    void applyFMediana();
    void showNewFDifest();
    void applyFDifest();
    void showNewFVecinos();
    void applyFVecinos();
    void showNewFDef();
    void applyFDef();
    void showNewFGauss();
    void applyFGauss();


    //Practica 3 y mas
    void selectHerramienta(); //al checkear una herramienta, descheckea las demas

    void espejoVertical();
    void espejoHorizontal();
    void traspuesta();
    void rotar90();
    void rotar180();
    void rotar270();
};

#endif // ASHOT_H
