#include "imagen.h"
//#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

Imagen::Imagen() {
    qimage = new QImage();
    if (this->isNull())
        err = 1;
    else
        err = 0;
    update();
}




Imagen::Imagen(QString fileName) {
    qimage = new QImage(fileName);
    this->file = fileName;
    if (this->isNull())
        err = 1;
    else
        err = 0;
    update();
}



int Imagen::error() {
    return err;
}

void Imagen::createHist() {
    if (qimage->format() == 3) //8 bits indexado, monocromo
        Mniveles = 256;
    else if (qimage->format() == 4) //32 bits, RGB
        Mniveles = 256;
         else Mniveles = 2; //error

    Mniveles = 256;
    hist = new int [Mniveles];
    acum = new int [Mniveles];
    for (int i = 0; i < Mniveles; i++) {
        hist[i] = 0;
        acum[i] = 0;
    }

    if ((qimage->format() != 3) && (qimage->format() != 4)) { //8-bit indexado, monocromo, o RGB 32 bits
        err = 1; //error
        return;
    }

    err = 0;
    QRgb pix; //= this->qimage->pixel(4, 5); //un valor cualquiera
    int val; //= qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;

    //se crea hist
    for (int i = 0; i < this->width(); i++) // igual: qimage->width()
        for (int j = 0; j < this->height(); j++) {
            pix = qimage->pixel(i, j);
            val = qGray(pix);
            hist[val]++; //frecuencia absoluta de nivel de gris val
        }

    //se crea acum
    int suma = 0;
    for (int i = 0; i < this->M(); i++) {
        suma += hist[i];
        acum[i] = suma;
    }
}




void Imagen::update() {
    createHist();
}



void Imagen::transformar(int * vout) {
    //QRgb pix = imagen->qimage->pixel(4, 5);
    int vin; // = qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;
    for (int i = 0; i < width(); i++)
        for (int j = 0; j < height(); j++) {
            vin = qGray(qimage->pixel(i, j));
            qimage->setPixel(i, j, vout[vin]); //ya se garantiza que el tamaño de vout es de M elementos
        }

    update();
}

int Imagen::M() { //numero total de niveles
    return Mniveles;
}

int Imagen::max() {
    int maximo = hist[0];
    for (int i = 0; i < M(); i++)
        if (maximo < hist[i])
            maximo = hist[i];
    return maximo;
}

int Imagen::min() {
    int minimo = hist[0];
    for (int i = 0; i < M(); i++)
        if ((minimo > hist[i]) && (hist[i] > 0)) //no se cuenta el 0
            minimo = hist[i];
    return minimo;
}

double Imagen::brillo() { //media de valores
    if (err) return 0.0;

    double bri = 0;
    for (int i = 0; i < M(); i++)
        bri += i * hist[i];

    bri = bri/this->size();
    return bri;
}

double Imagen::contraste() {
    if (err) return 0.0;

    double contr = 0.0;
    for (int i = 0; i < M(); i++)
        contr += pow(i - brillo(), 2) * hist[i];

    //cout << "contr: " << contr << endl;

    contr *= 1.0/ (double) size();
    //cout << "contr: " << contr << endl;
    contr = sqrt(contr);
    //cout << "contr: " << contr << endl;
    return contr;
}

double Imagen::entropia() {
    if (err) return 0.0;

   /* cout << "rel: " << hRel(3);
    for (int i = 0; i < M(); i++) {
        cout << "hist" << i << ": " << hist[i] << ", rel: " << hRel(i) << endl;
    }*/

    double entr = 0.0;
    for (int i = 0; i < M(); i++)
        if (hRel(i) > 0) //para que no de error matematico
            entr += hRel(i) * log2(hRel(i));

    return (- entr);
}


int Imagen::hAbs(int i) {
    return hist[i];
}

double Imagen::hRel(int i) { //frecuencia relativa del nivel i (para histograma relativo)
    return ((double) hist[i] / (double) size());
}

int Imagen::hAcum(int i) {
    return acum[i];
}


double Imagen::hAcumNorm(int i) {
    return ((double) acum[i] / (double) size());
}


int Imagen::height() {
    return qimage->height();
}

int Imagen::width() {
    return qimage->width();
}

int Imagen::size() { //numero total de pixeles
    return width() * height();
}

/* int Imagen::pixel(int x, int y);
void Imagen::setPixel(int x, int y, int value) {
    qimage.setPixel(x, y, value);
}*/


QImage Imagen::qImage() {
    return *qimage;
}

QString Imagen::fileName() {
    return file;
}

QString Imagen::extension() {
    return file;
}

QString Imagen::formato() {
    QString format;
    switch (qimage->format()) {
        case 0: format = "The image is invalid";
        case 1: format = "The image#include <iostream> is stored using 1-bit per pixel. Bytes are packed with the most significant bit (MSB) first."; break;
    case 2: format = "The image is stored using 1-bit per pixel. Bytes are packed with the less significant bit (LSB) first."; break;
        case 3: format = "The image is stored using 8-bit indexes into a colormap."; break;
        case 4: format = "The image is stored using a 32-bit RGB format (0xffRRGGBB)."; break;
    case 5: format = "The image is stored using a 32-bit ARGB format (0xAARRGGBB)."; break;
    case 6: format = "The image is stored using a premultiplied 32-bit ARGB format (0xAARRGGBB)"; break;
        default: format = "formato desconocido";
    }

    return format;
}

bool Imagen::isNull() {
    if (qimage == NULL)
        return true;
    else
        if (qimage->isNull())
            return true;
        else
            return false;
}
