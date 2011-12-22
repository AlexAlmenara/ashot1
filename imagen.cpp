#include "imagen.h"
//#include <stdio.h>
#include <math.h>
//#include <iostream>
#include <QList>

using namespace std;

Imagen::Imagen() {
    qimage = QImage();
    if (this->isNull())
        err = 1;
    else
        err = 0;
    update();
}




Imagen::Imagen(QString fileName) {
    qimage = QImage(fileName);
    this->name = fileName;
    if (this->isNull())
        err = 1;
    else
        err = 0;
    update();
}


Imagen::Imagen(QImage image, QString fileName) {
    setQImage(image);
    if (fileName != "")
        name = fileName;
    //update(); //ya lo hace setQImage
}




int Imagen::error() {
    return err;
}

void Imagen::createHist() {
    if (qimage.format() == 3) //8 bits indexado, monocromo
        Mniveles = 256;
    else if (qimage.format() == 4) //32 bits, RGB
        Mniveles = 256;
         else Mniveles = 2; //error

    Mniveles = 256;
    hist = new int [Mniveles];
    acum = new int [Mniveles];
    for (int i = 0; i < Mniveles; i++) {
        hist[i] = 0;
        acum[i] = 0;
    }

    if ((qimage.format() != 3) && (qimage.format() != 4)) { //8-bit indexado, monocromo, o RGB 32 bits
        err = 1; //error
        return;
    }

    err = 0;
    QRgb pix; //= this->qimage->pixel(4, 5); //un valor cualquiera
    int val; //= qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;

    //se crea hist
    for (int i = 0; i < this->width(); i++) // igual: qimage->width()
        for (int j = 0; j < this->height(); j++) {
            pix = qimage.pixel(i, j);
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

    //actualizacion para diagonal (perfil)
    if (width() == height()) { //imagen cuadrada
        n_diag = width();
        dist_diag = 1;
        return;
    }

    if (width() > height()) { //ancho mayor
        dist_diag = (double) width() / (double) height();
        n_diag = width();
        return;
    }

    //else: mayor alto
    dist_diag = (double) height() / (double) width();
    n_diag = height();
}


int Imagen::nDiagonal() { //n_diag
    return n_diag;
}


void Imagen::transformar(int * vout) {
    //QRgb pix = imagen->qimage->pixel(4, 5);
    int vin; // = qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;
    for (int i = 0; i < width(); i++)
        for (int j = 0; j < height(); j++) {
            vin = qGray(qimage.pixel(i, j));
            qimage.setPixel(i, j, vout[vin]); //ya se garantiza que el tamaño de vout es de M elementos
        }

    update();
}



void Imagen::setQImage(QImage image) {
    qimage = image;
    update();
    //cuidado no se ha puesto el nombre de la imagen
}

void Imagen::setFileName(QString fileName) {
    name = fileName;
}


void Imagen::pegarImagen(Imagen image, QPoint p1) {
    if (image.size() == this->size()) {
        *this = image;
        return;
    }

    //if (p2 == QPoint(-1, -1)) //si es valor por defecto
    //    p2 = QPoint(p1.x() + image.width(), p1.y() + image.height()); //se pone hasta el final de image

    int vin;
    int x, y; //posicion en this.qimage
    for (int i = 0; i < image.width(); i++) //recorre la imagen
        for (int j = 0; j < image.height(); j++) {
            vin = qGray(image.qimage.pixel(i, j));
            x = p1.x() + i;
            y = p1.y() + j;
            if ((x < this->qimage.width()) && (y < this->qimage.height())) //si no se sale de la imagen original
                this->qimage.setPixel(x, y, vin);
        }

    update();
}


int Imagen::M() { //numero total de niveles
    return Mniveles;
}

int Imagen::maxh() {
    int maximo = hist[0];
    for (int i = 0; i < M(); i++)
        if (maximo < hist[i])
            maximo = hist[i];
    return maximo;
}


int Imagen::moda() {
    int maximo = hist[0];
    int vin = 0;
    for (int i = 0; i < M(); i++)
        if (maximo < hist[i]) {
            maximo = hist[i];
            vin = i;
        }

    return vin;
}



double Imagen::mediana() {
    int suma = 0;
    int i;
    for (i = 0; i < M(); i++) {
        suma += hist[i];
        if (suma > (this->size() / 2)) //ya paso de la mitad
            return i - 1;
    }
    return (double) i; //para asegurar que retorne, aunque no tendria xq que pasar por aqui
}



int Imagen::maxRango() {
    for (int i = M() - 1; i >= 0; i--)
        if (hist[i] > 0)
            return i; //devuelve el primer nivel distinto de 0
    return 0; //por seguridad, si todos son 0
}

int Imagen::minRango() {
    for (int i = 0; i < M(); i++)
        if (hist[i] > 0)
            return i; //devuelve el primer nivel distinto de 0
    return 0; //por seguridad, si todos son 0
}

double Imagen::brillo() { //media de valores
    if (err) return 0.0;

    double bri = 0;
    for (int i = 0; i < M(); i++)
        bri += i * hist[i];

    bri = bri/ (double) this->size();
    return bri;
}

double Imagen::contraste() {
    if (err) return 0.0;

    double bri = brillo();
    double contr = 0.0;
    for (int i = 0; i < M(); i++)
        contr += pow(i - bri, 2) * hist[i];

    contr /= (double) size();
    contr = sqrt(contr);
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



int Imagen::negativo(int vin) {
    return M() - 1 - vin; //inverso de vin: 255 - vin
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
    return qimage.height();
}

int Imagen::width() {
    return qimage.width();
}

int Imagen::size() { //numero total de pixeles
    return width() * height();
}

/* int Imagen::pixel(int x, int y);
void Imagen::setPixel(int x, int y, int value) {
    qimage.setPixel(x, y, value);
}


QImage Imagen::qImage() {
    return *qimage;
}*/


/*int Imagen::gray(int i) {
    return qGray(qimage.color(i)); //CUIDADO: esto es el indice de la tabla!!
}*/

int Imagen::gray(int x, int y) {
    return qGray(qimage.pixel(x, y));
}


QPoint Imagen::posPerfil(int i) {
    if (width() == height()) //imagen cuadrada
        return QPoint(i, i);

    if (width() > height()) //ancho mayor
        return QPoint(i, (int) ((double) i / dist_diag));

    //else: mayor alto
    return QPoint((int) ((double) i / dist_diag), i);

}


int Imagen::perfil(int i) {
    if (width() == height()) //imagen cuadrada
        return gray(i, i);

    if (width() > height()) //ancho mayor
        return gray(i, (int) ((double) i / dist_diag));

    //else: mayor alto
    return gray((int) ((double) i / dist_diag), i);

}


int Imagen::dperfil(int i) {
    if ((i == 0) || (i == nDiagonal() - 1))
        return perfil(i);

    return perfil(i + 1) - perfil(i);
}



int Imagen::perfilSuave(int i) {
    if (i == 0)
        return (int) ((double) (perfil(0) + perfil(1)) / 2.0);
    else
        if (i == nDiagonal() - 1)
           return (int) ((double) (perfil(i - 1) + perfil(i)) / 2.0);
        else
            return (int) ((double) (perfil(i - 1) + perfil(i) + perfil(i + 1)) / 3.0);
}


int Imagen::dperfilSuave(int i) {
    if ((i == 0) || (i == nDiagonal() - 1))
        return perfilSuave(i);

    return perfilSuave(i + 1) - perfilSuave(i);
}


QString Imagen::path() {
    return name; //ruta absoluta
}


QString Imagen::fileName() {
    return name.section('/', -1);
}

QString Imagen::extension() {
    return name.section('.', -1);
}

QString Imagen::formato() {
    QString format;
    switch (qimage.format()) {
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
    /*if (qimage == NULL)
        return true;
    else*/
    if (qimage.isNull())
        return true;
    else
        return false;
}





//pract2: renombramiento de funciones anteriores pero para una region
double Imagen::brillo(int x1, int y1, int x2, int y2) {
    int suma = 0;
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++)
            suma += this->gray(i, j);

    int N = (x2 - x1) * (y2 - y1); //numero de pixeles de la region
    return (double) suma / (double) N;
}


double Imagen::contraste(int x1, int y1, int x2, int y2) {
    int bri = this->brillo(x1, y1, x2, y2);
    double suma = 0.0;
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++)
            suma += pow((gray(i, j) - bri), 2);

    int N = (x2 - x1) * (y2 - y1); //numero de pixeles de la region
    return sqrt((double) suma / (double) N);

}


double Imagen::mediana(int x1, int y1, int x2, int y2) {
    int N = (x2 - x1) * (y2 - y1); //numero de pixeles de la region

    //creacion de lista de los niveles ORDENADOS
    QList<int> lista; //empieza vacia
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++)
            lista.append(gray(i, j)); //inserta al final. tambien se puede usar: lista << valor

    qSort(lista); //ordena la lista
    //lista.size() == N

    //calcula mediana
    if (N % 2) //si N es impar
        return (double) lista.at((N + 1) / 2); //el del medio
    else
        return (double) ((lista.at(N/2) + lista.at((N/2) + 1)) / 2); //es par: la media de los dos centrales

}


int Imagen::moda(int x1, int y1, int x2, int y2) {
    int * hist2 = new int [this->M()];
    for (int i = 0; i < M(); i++)
        hist2[i] = 0;


    //se crea hist
    int valor;
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) {
            valor = this->gray(i, j);
            hist2[valor]++; //frecuencia absoluta de nivel de gris val
        }

    //calcula moda (valor de gris de maxima frecuencia)
    int maximo = hist2[0];
    valor = 0;
    for (int i = 0; i < M(); i++)
        if (maximo < hist2[i]) {
            maximo = hist2[i];
            valor = i;
        }

    return valor;
}
