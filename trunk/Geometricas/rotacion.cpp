#include "rotacion.h"
#include "ui_rotacion.h"

#include <QGenericMatrix>
#include <math.h>


ostream &operator << (ostream &sout, FPoint &p) {
   sout << "(" << p.getX() << ", " << p.getY() << ")";
   return sout;
}


double toRad(double deg) { //pasa angulo de Deg a Rad
    return (deg * 2 * PI) / 360;
}


Rotacion::Rotacion(QWidget *parent, Imagen imagen) :
    QWidget(parent),
    ui(new Ui::Rotacion)
{
    ui->setupUi(this);

    imagenOrig = imagen;
    imagenAux = imagen;

    ui->dSpinBoxAngulo->setRange(-360.0, 360.0);
    ui->dSpinBoxAngulo->setValue(0.0);
    ui->hSliderAngulo->setRange(-360, 360);
    ui->hSliderAngulo->setValue(0);

    ui->comboBoxInterpolacion->setCurrentIndex(BILINEAL);
    ui->comboBoxTransf->setCurrentIndex(INDIRECTA);

    //conexiones
    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->hSliderAngulo, SIGNAL(valueChanged(int)), this, SLOT(intToDouble(int)));
    connect(this, SIGNAL(updateSpinBox(double)), ui->dSpinBoxAngulo, SLOT(setValue(double)));

    connect(ui->dSpinBoxAngulo, SIGNAL(valueChanged(double)), this, SLOT(DoubleToInt(double)));
    connect(this, SIGNAL(updateSlider(int)), ui->hSliderAngulo, SLOT(setValue(int)));

    connect(ui->dSpinBoxAngulo, SIGNAL(valueChanged(double)), this, SLOT(cambiarImagen()));
    connect(ui->comboBoxInterpolacion, SIGNAL(currentIndexChanged(int)), this, SLOT(cambiarImagen()));
    connect(ui->comboBoxTransf, SIGNAL(currentIndexChanged(int)), this, SLOT(cambiarImagen()));
}

Rotacion::~Rotacion()
{
    delete ui;
}

void Rotacion::intToDouble(int value) {
    double convertedValue = (double) value;
    emit(updateSpinBox(convertedValue)); //se emite signal
}

void Rotacion::DoubleToInt(double value) {
    int convertedValue = (int) value;
    emit(updateSlider(convertedValue)); //se emite signal
}

FPoint Rotacion::firstTD(int x, int y) {
    //double * vect = new double [4];
    //matriz: transformacion directa
    QGenericMatrix<2, 2, double> matTransf; //(vect); //(N = columna, M = fila)
    matTransf(0, 0) = cos(ang); matTransf(0, 1) = sin(ang); //primera fila (i = fila, j = columna)
    matTransf(1, 0) = -sin(ang); matTransf(1, 1) = cos(ang);

    QGenericMatrix<1, 2, double> matMultp; //matriz columna que multiplica
    QGenericMatrix<1, 2, double> matResult; //matriz columna de resultado

    matMultp(0, 0) = x; matMultp(1, 0) = y;
    matResult = matTransf * matMultp;
    FPoint p; p.setX(matResult(0, 0)); p.setY(matResult(1, 0)); //el - es para invertir el eje y
    //cout << "\nX = " << Ep.getX() << ", Y = " << Ep.getY() << endl;
    return p;
}

FPoint Rotacion::tD(int x, int y) { //transformacion directa: (x, y) de imagenOrig, devuelve (xp, yp) de imagen rotada
    FPoint p = firstTD(x, y);

    //mover el eje de coordenadas para que se quede la imagen dentro del paralelogramo
    p.setX(p.getX() + Ep.getX() - 1);
    p.setY(p.getY() + Ep.getY() - 1);
    return p;
}


FPoint Rotacion::firstTI(int xp, int yp) {
    //matriz: transformacion indirecta
    QGenericMatrix<2, 2, double> matTransf; //(vect); //(N = columna, M = fila)
    matTransf(0, 0) = cos(ang); matTransf(0, 1) = -sin(ang); //primera fila (i = fila, j = columna)
    matTransf(1, 0) = sin(ang); matTransf(1, 1) = cos(ang);

    QGenericMatrix<1, 2, double> matMultp; //matriz columna que multiplica
    QGenericMatrix<1, 2, double> matResult; //matriz columna de resultado

    matMultp(0, 0) = xp; matMultp(1, 0) = yp;
    matResult = matTransf * matMultp;
    FPoint p; p.setX(matResult(0, 0)); p.setY(matResult(1, 0)); //el - es para invertir el eje y
    return p;
}

FPoint Rotacion::tI(int xp, int yp) { //transformacion indirecta: (xp, yp) de imagen rotada, devuelve (x, y) de imagenOrig
    FPoint p(xp, yp);

    //deshacer el movimiento del eje de coordenadas (Ver tD()): asi se sale del paralelogramo
    p.setX(p.getX() - Ep.getX());
    p.setY(p.getY() - Ep.getY());

    p = firstTI(p.getX(), p.getY());
    return p;
}


void Rotacion::initRotacion() {
    ang = toRad(ui->dSpinBoxAngulo->value()); //angulo

    //calculo de las 4 esquinas en la imagen rotada
    Ep = firstTD(0, 0);
    Fp = firstTD(imagenOrig.width() - 1, 0);
    Gp = firstTD(imagenOrig.width() - 1, imagenOrig.height() - 1);
    Hp = firstTD(0, imagenOrig.height() - 1);

    //cout << endl << "first: E = " << Ep << ", F = " << Fp << ", G = " << Gp << ", H = " << Hp << flush;

    if ( ((ang >= R270) && (ang <= R360)) || ((ang >= -R90) && (ang <= 0)) ) { //0..90 sentido horario
        double x = Hp.getX(); //todos los puntos: + (-H.x(), 0)
        Ep.setX(Ep.getX() - x);
        Fp.setX(Fp.getX() - x);
        Gp.setX(Gp.getX() - x);
        Hp.setX(Hp.getX() - x);

        //dimensiones de imagen rotada
        anchura = Fp.getX() + 1;
        altura = Gp.getY() + 1;
        //cout << "\nang = 0..90" << flush;
    }
    else
        if ( ((ang >= R180) && (ang <= R270)) || ((ang >= -R180) && (ang <= -R90)) ) { //90..180 sentido horario
            double x = Gp.getX(); double y = Hp.getY(); //todos los puntos: + (-G.x(), -H.y())
            Ep.setX(Ep.getX() - x);
            Fp.setX(Fp.getX() - x);
            Gp.setX(Gp.getX() - x);
            Hp.setX(Hp.getX() - x);

            Ep.setY(Ep.getY() - y);
            Fp.setY(Fp.getY() - y);
            Gp.setY(Gp.getY() - y);
            Hp.setY(Hp.getY() - y);

            //dimensiones de imagen rotada
            anchura = Ep.getX() + 1;
            altura = Fp.getY() + 1;
            //cout << "\nang = 90..180" << flush;
        }
        else
            if ( ((ang >= R90) && (ang <= R180)) || ((ang >= -R270) && (ang <= -R180)) ) { //180..270 sentido horario
                double x = Fp.getX(); double y = Gp.getY(); //todos los puntos: + (-F.x(), -G.y())
                Ep.setX(Ep.getX() - x);
                Fp.setX(Fp.getX() - x);
                Gp.setX(Gp.getX() - x);
                Hp.setX(Hp.getX() - x);

                Ep.setY(Ep.getY() - y);
                Fp.setY(Fp.getY() - y);
                Gp.setY(Gp.getY() - y);
                Hp.setY(Hp.getY() - y);

                //dimensiones de imagen rotada
                anchura = Hp.getX() + 1;
                altura = Ep.getY() + 1;
            }
            else {
                //if ( ((ang >= 0) && (ang <= 90)) || ((ang >= -360) && (ang <= -270)) ) //270..360 sentido horario
                double y = Fp.getY(); //todos los puntos: + (0, -F.y())
                Ep.setY(Ep.getY() - y);
                Fp.setY(Fp.getY() - y);
                Gp.setY(Gp.getY() - y);
                Hp.setY(Hp.getY() - y);

                //dimensiones de imagen rotada
                anchura = Gp.getX() + 1;
                altura = Hp.getY() + 1;
            }

    imagenAux.setQImage(imagenOrig.qimage.copy(QRect(0, 0, anchura, altura))); //copia imagen a partir de region de seleccion
    /*cout << "\ndepues: E = " << Ep << ", F = " << Fp << ", G = " << Gp << ", H = " << Hp << endl << flush;
    cout << "anchura = " << anchura << ", altura = " << altura << endl << flush;
    cout << "anchura = " << imagenAux.width() << ", altura = " << imagenAux.height() << endl << flush;
    FPoint E = tI(Ep.getX(), Ep.getY());
    FPoint F = tI(Fp.getX(), Fp.getY());
    FPoint G = tI(Gp.getX(), Gp.getY());
    FPoint H = tI(Hp.getX(), Hp.getY());
    cout << "orig: E = " << E << ", F = " << F << ", G = " << G << ", H = " << H << endl << flush; */
}


void Rotacion::cambiarImagen() {
    //cout << "\ncambiar imagen" << flush;
    imagenAux = imagenOrig;

    initRotacion(); //crea la imagenAux (paralelogramo)

    if (ui->comboBoxTransf->currentIndex() == DIRECTA) {
        ui->comboBoxInterpolacion->setEnabled(false); //no se usa metodo de interpolacion

        int Xp, Yp; // (X', Y')
        //inicializa la imagen rotada
        for (Xp = 0; Xp < imagenAux.width(); Xp++) // reocorre la imagen rotada (mapeado inverso)
            for (Yp = 0; Yp < imagenAux.height(); Yp++) { // (X', Y')
                imagenAux.setPosFondo(Xp, Yp, TRANSP);
                //imagenAux.qimage.setPixel(Xp, Yp, COLOR_TRANSP);
            }
        imagenAux.update(); //pone toda la imagen a transparente

        //transformacion directa
        for (int X = 0; X < imagenOrig.width(); X++) // reocorre la imagen original
            for (int Y = 0; Y < imagenOrig.height(); Y++) { // (X, Y)
                FPoint p = tD(X, Y); //transformacion directa
                Xp = (int) (p.getX() + 0.5); //(X', Y'): redondeo al mas cercano
                Yp = (int) (p.getY() + 0.5);
                //if ((Xp < imagenAux.width()) && (Yp < imagenAux.height()) && (Xp >= 0) && (Yp >= 0) ) {//si el punto (X', Y') de imagenAux NO se sale del rango
                imagenAux.setPosFondo(Xp, Yp, imagenOrig.posFondo(X, Y));
                imagenAux.qimage.setPixel(Xp, Yp, imagenOrig.gray(X, Y));
                //cout << "\nwidth = " << imagenAux.width() << ", height = " << imagenAux.height() << flush;
                //cout << "\nXp = " << Xp << ", Yp = " << Yp << flush;
            }
    } //if

    else { //transformacion INDIRECTA: la deseable
        ui->comboBoxInterpolacion->setEnabled(true);
        for (int Xp = 0; Xp < imagenAux.width(); Xp++) // reocorre la imagen rotada (mapeado inverso)
            for (int Yp = 0; Yp < imagenAux.height(); Yp++) { // (X', Y')
                FPoint xy = tI(Xp, Yp); //transformacion indirecta: devuelve punto de imagenOrig (aun DOUBLE)

                int X = (int) xy.getX(); //redondeo hacia abajo == floor(x);
                int Y = (int) xy.getY();
                //tambien: X + 1, Y + 1
                //imagenAux.qimage.setPixel(Xp, Yp, 0); //prueba

                if ((X >= imagenOrig.width()) || (Y >= imagenOrig.height()) || (X < 0) || (Y < 0) ) {//si el punto (X, Y) de imagenOrig se sale del rango
                    imagenAux.setPosFondo(Xp, Yp, TRANSP); //fuera de la imagen rotada: transparente
                }
                else {
                    double p = xy.getX() - X;
                    double q = xy.getY() - Y;
                    int gris;
                    //si esta dentro de imagenOrig: asignar (Xp, Yp) segun metodo interpolacion
                    if (ui->comboBoxInterpolacion->currentIndex() == VMP) { //Vecino mas proximo
                        int vecinoX, vecinoY;
                        if ((p > 0.5) && (X + 1 < imagenOrig.width())) //si se acerca mas a X+1 y X+1 no se sale de la imagen
                           vecinoX = X + 1;
                        else
                            vecinoX = X;

                        if ((q > 0.5) && (Y + 1 < imagenOrig.height())) //si se acerca mas a Y+1 y Y+1 no se sale de la imagen
                           vecinoY = Y + 1;
                        else
                            vecinoY = Y;

                        gris = imagenOrig.gray(vecinoX, vecinoY);
                    }
                    else { //Interpolacion Bilineal
                        int A = imagenOrig.gray(X, Y);
                        if ((X + 1 < imagenOrig.width()) && (Y + 1 < imagenOrig.height())) { //no se salen de la imagen
                            int B = imagenOrig.gray(X + 1, Y);
                            int C = imagenOrig.gray(X, Y + 1);
                            //cout << ", dentro todo" << flush;
                            int D = imagenOrig.gray(X + 1, Y + 1);
                            gris = (int) ((C + (D - C) * p + (A - C) * q + (B + C - A - D) * p * q) + 0.5); //redondeo al mas cercano
                        }
                        else
                            if ((X + 1 >= imagenOrig.width()) && (Y + 1 >= imagenOrig.height())) //se salen de la imagen
                                gris = A; //es solo (X, Y)
                            else
                                if (X + 1 >= imagenOrig.width()) { //X+1 se sale
                                    int C = imagenOrig.gray(X, Y + 1);
                                    gris = (int) ((A + (C - A) * q) + 0.5); //Y..Y+1
                                }
                                else {
                                    int B = imagenOrig.gray(X + 1, Y);
                                    gris = (int) ((A + (B - A) * p) + 0.5); //X..X+1 (Y+1 se sale)
                                }
                    } //fin bilineal

                    if (gris < 0)
                        gris = 0;
                    else
                        if (gris > imagenAux.M() - 1)
                            gris = imagenAux.M() - 1;

                    imagenAux.qimage.setPixel(Xp, Yp, gris);
                } //else if */
            } //for */
    }

    /*imagenAux.qimage.setPixel(Ep.getX(), Ep.getY(), 255); //pintar esquinas
    imagenAux.qimage.setPixel(Fp.getX(), Fp.getY(), 255);
    imagenAux.qimage.setPixel(Gp.getX(), Gp.getY(), 255);
    imagenAux.qimage.setPixel(Hp.getX(), Hp.getY(), 255);*/

    //imagenAux.setFondo(fondo);
    imagenAux.update(); //actualiza tambien todo lo que sea transparente
    emit(changed());
}

void Rotacion::aceptar() {
    cambiarImagen();
    //aShot actualiza y despues:
    close();
    emit(acepted());
    delete this;
}

void Rotacion::cancelar() {
    imagenAux = imagenOrig;
    close();
    emit(changed());
    delete this;
}
