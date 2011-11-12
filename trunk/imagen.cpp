#include "imagen.h"

Imagen::Imagen() {
    image = new QImage();
    if (this->isNull())
        err = 1;
    else
        err = 0;
    createVect();
}




Imagen::Imagen(QString fileName) {
    image = new QImage(fileName);
    this->file = fileName;
    if (this->isNull())
        err = 1;
    else
        err = 0;
    createVect();
}


int Imagen::error() {
    return err;
}

void Imagen::createVect() {
    if (image->format() != 3) { //8-bit indexado, monocromo
        err = 1; //error
    }

    Mniveles = 256;
    vect = new int [Mniveles];
}

int Imagen::M() { //numero total de niveles
    return Mniveles;
}

int Imagen::max() {
    int maximo = vect[0];
    for (int i = 0; i < M(); i++)
        if (maximo < vect[i])
            maximo = vect[i];
    return maximo;
}

int Imagen::min() {
    int minimo = vect[0];
    for (int i = 0; i < M(); i++)
        if (minimo > vect[i])
            minimo = vect[i];
    return minimo;
}

int Imagen::brillo() {
    return 2;
}

int Imagen::contraste() {
    return 3;
}

int Imagen::entropia() {
    return 4;
}

int Imagen::height() {
    return image->height();
}

int Imagen::width() {
    return image->width();
}

int Imagen::size() { //numero total de pixeles
    return width() * height();
}

/* int Imagen::pixel(int x, int y);
void Imagen::setPixel(int x, int y, int value) {
    image.setPixel(x, y, value);
} */

QString Imagen::fileName() {
    return file;
}

QString Imagen::extension() {
    return file;
}

QString Imagen::formato() {
    QString format;
    switch (image->format()) {
        case 0: format = "The image is invalid";
        case 1: format = "The image is stored using 1-bit per pixel. Bytes are packed with the most significant bit (MSB) first."; break;
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
    if (image == NULL)
        return true;
    else
        if (image->isNull())
            return true;
        else
            return false;
}
