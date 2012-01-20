#ifndef IMAGEN_H
#define IMAGEN_H

#include <QString>
#include <QImage>
//#include <QtGui>

//#include <QMainWindow>
//#include <QPrinter>

#define OPACO 255 //es pixel de imagen para mostrar en histograma
#define TRANSP 0 //transparente: se considera fuera de la imagen, no se muestra en histograma

#define COLOR_TRANSP 125 //color para transparente
#define COLOR_APLANADO 255 //color que al aplanar convierte el transparente en plano

class Imagen {

private:
    int * hist; //histograma: frecuencias absolutas de los niveles. Si fuera COLOR: 3 hist
    int * acum; //histograma acumulado: frec acumuladas

    int Mniveles; //numero total de niveles
    QString name; //nombre de la imagen (ruta absoluta)
    int err; //error: imagen null, o se usa un formato que no hemos albergado para vect

    double dist_diag; //numero de pixeles seguidos en la diagonal por ser imagen cuadrada
    int n_diag; //numero de pixeles de la diagonal

    int ** fondo; //matriz para decir que pixels son fondo, para no mostrar en histograma

    //bool firstCreated; //para controlar el initFondo() bien

    void createHist();

public:
    Imagen(QString fileName = "");
    Imagen(QImage image, QString fileName = ""); //puede ser QImage de distinta ubicacion a fileName
    //Imagen(QImage qimage); //estaria bien este constructor si al menos QImage tuviera el metodo fileName() xDD

    QImage qimage;

    int nDiagonal(); //n_diag
    int error(); //se saca el error para que lo lea aShot y sace un MessageBox
    int M(); //numero total de niveles
    int maxh(); //maximo valor de frecuencia (sin saber de q nivel es, es solo para saber dibujar histograma
    int moda(); //(max_vin) moda estadistica: nivel de gris correspondiente a maxh, el nivel mas frecuente
    double mediana(); //mediana estadistica: estando ordenados los niveles, devuelve el nivel que este a la mitad
    int maxRango(); //rango dinamico: [min, max]
    int minRango();
    double brillo();
    double contraste();
    double entropia();

    int hAbs(int i); //frecuencia absoluta del nivel i
    double hRel(int i); //frecuencia relativa del nivel i
    int hAcum(int i); //frecuencia acumulada del nivel i
    double hAcumNorm(int i); //frecuencia acumulada NORMALIZADO del nivel i

    int negativo(int vin); //negativo del nivel vin

    int height();
    int width();
    int size();

    void update(); //actualiza info asociada a imagen: vector hist
    void transformar(int * vout); //trasnforma una imagen con una tabla LUT dada, es decir, vector vout
    void setQImage(QImage image);
    void setFileName(QString fileName);

    void pegarImagen(Imagen image, QPoint p1); //pega image desde punto p1. donde se salga, por supuesto no hace nada. // QPoint p2 = QPoint(-1, -1));

    //int gray(int i);
    int gray(int x, int y); //devuelve nivel de gris de posicion dada

    int perfil(int i);  //funcion perfil (diagonal)
    int dperfil(int i); //derivada de la funcion perfil
    int perfilSuave(int i);  //perfil suavizado: media de pixel i sus dos vecinos
    int dperfilSuave(int i);
    QPoint posPerfil(int i); //devuelve posicion de imagen de perfil i
    //void setPixel(int value);

    //QImage qImage();
    QString path();
    QString fileName();
    QString extension();
    QString formato();
    bool isNull();


    //pract2: renombramiento de funciones anteriores pero para una region
    double brillo(int x1, int y1, int x2, int y2);
    double contraste(int x1, int y1, int x2, int y2);
    double mediana(int x1, int y1, int x2, int y2);
    int moda(int x1, int y1, int x2, int y2);

    //pract3:
    void initFondo(); //inicializa fondo
    void setFondo(int ** fo); //crea una nueva matriz de fondo
    void setPosFondo(int i, int j, int valor); //pone valor en posicion de matriz de fondo
    int posFondo(int i, int j); //devuelve valor de posicion matriz de fondo
    void aplanar(); //vacia matriz de fondo, todos los pixeles se consideran de la imagen para mostrar
    bool esTransp(int i, int j); //true si pixel es transparente

    void rotar90(); //rota la imagen 90 grados en sentido agujas del reloj

};

#endif // IMAGEN_H
