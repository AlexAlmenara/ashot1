#include "fvecinos.h"
#include "ui_fvecinos.h"
#include <QtAlgorithms>
#include "stdio.h"

FVecinos::FVecinos(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::FVecinos)
{
    ui->setupUi(this);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->spinBoxTam, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));
    connect(ui->spinBoxK, SIGNAL(editingFinished()), this, SLOT(cambiarImagen()));

    imagenOriginal = image;
    imagenAux = image;

    ui->spinBoxTam->setRange(1, 99);
    ui->spinBoxTam->setValue(3); //por defecto ventana 3x3
    ui->spinBoxK->setRange(1, 99);
    ui->spinBoxK->setValue(5);
}

FVecinos::~FVecinos()
{
    delete ui;
}


bool comparaAbs(const int & v1, const int & v2) { //funcion para ordenar por valores absolutos
    return abs(v1) < abs(v2);
}


void FVecinos::cambiarImagen() {
    ui->labelTerminado->setText("Filtrando...");
    imagenAux =  imagenOriginal;

    //comprueba que sea tamaño impar y si no lo cambia
    int N = ui->spinBoxTam->value(); //tamaño de ventana: NxN
    if (!(N % 2)) { //no es impar
        N++;
        ui->spinBoxTam->setValue(N);
    }

    ui->spinBoxK->setRange(1, N * N);
    if (ui->spinBoxK->value() > N * N) //controlar para que k no se pase del tamaño de ventana
        ui->spinBoxK->setValue(N); //por ejemplo

    int K = ui->spinBoxK->value();

    //filtro
    QList<int> lista; //lista de restas ordenados por.
    int nborde = (int) ((double) N / 2.0); //redondeado hacia abajo
    int resta;
    //int u = 5;
    for (int u = nborde; u <= imagenOriginal.width() - 1 - nborde; u++) //recorre la imagen sin pasar por bordes
        for (int v = nborde; v <= imagenOriginal.height() - 1 - nborde; v++) {

            //printf("\ngray(u, v): %d\nventana 1:", imagenOriginal.gray(u, v));
            for (int i = -nborde; i <= nborde; i++) //recorre kernel (ventana)
                for (int j = -nborde; j <= nborde; j++) {
                    //printf(",  %d", imagenOriginal.gray(u + i, v + j));
                    resta = imagenOriginal.gray(u, v) - imagenOriginal.gray(u + i, v + j); //resta para comparar distancia de valores
                    lista.append(resta);
                }

            qSort(lista.begin(), lista.end(), comparaAbs); //ordena segun valor absoluto

            /*for (int k = 0; k < lista.size(); k++)
                printf(", %d", lista.at(k));
            printf("\n"); */

            //halla media
            //printf("\nseleccionados:");
            int media = 0; //valor de la media
            for (int k = 0; k < K; k++) { //coge los k primeros
                media += (- lista[k] + imagenOriginal.gray(u, v)); //del valor resta en lista, recupera el valor gris de vecino y lo suma
                //printf(", %d", abs(lista[k]) + imagenOriginal.gray(u, v));
            }

            media = (int) (((double) media / (double) K) + 0.5);
            //printf("\nmedia: %d\n", media);
            /*if (media < 0) {
                printf("\nmedia: %d\n", media);
                media = 0;
            }
            else
                if (media > imagenAux.M() - 1) {
                    printf("\nmedia: %d\n", media);
                    for (int k = 0; k < K; k++) //coge los k primeros
                       printf(", %d", abs(lista[k]) + imagenOriginal.gray(u, v));

                    printf("\ngray(u, v): %d\nventana 1:", imagenOriginal.gray(u, v));
                    for (int i = -nborde; i <= nborde; i++) //recorre kernel (ventana)
                        for (int j = -nborde; j <= nborde; j++)
                            printf(",  %d", imagenOriginal.gray(u + i, v + j));
                    media = imagenAux.M() - 1;
                }*/

            imagenAux.qimage.setPixel(u, v, media);
            lista.clear();
        }

    imagenAux.update();
    ui->labelTerminado->setText("Filtrado Terminado");
    emit(changed());
}

void FVecinos::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void FVecinos::cancelar() {
    imagenAux = imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    close();
    emit(changed());
    delete this;
}
