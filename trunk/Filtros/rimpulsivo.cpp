#include "rimpulsivo.h"
#include "ui_rimpulsivo.h"

RImpulsivo::RImpulsivo(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::RImpulsivo)
{
    ui->setupUi(this);

    setValues(image);

    connect(ui->ButtonCancelar, SIGNAL(clicked()), this, SLOT(cancelar()));
    connect(ui->ButtonAceptar, SIGNAL(clicked()), this, SLOT(aceptar()));

    connect(ui->doubleSpinBoxP, SIGNAL(editingFinished()), this, SLOT(cambiarImagen())); //cambios de spinbox cambian imagen directamente

    connect(ui->checkBoxHistR, SIGNAL(pressed()), this, SLOT(verHistRuido()));
    connect(ui->checkBoxImagenR, SIGNAL(pressed()), this, SLOT(verImagenRuido()));

    //cambiarImagen(); //por defecto ya se ve un ruido hecho
    //emit(changed());
}

RImpulsivo::~RImpulsivo()
{
    delete ui;
}


void RImpulsivo::setValues(Imagen image) {
    imagenOriginal = image;
    imagenAux = image;
    NO_RUIDO = 100;
    imagenRuidoCreada = false;
    matRuido = new int * [imagenOriginal.width()]; //creacion de matriz de ruido. fuente: http://c.conclase.net/curso/?cap=017
    for (int i = 0; i < imagenOriginal.width(); i++)
        matRuido[i] = new int[imagenOriginal.height()];

    ui->doubleSpinBoxP->setRange(0.0, 100.0); //porcentaje de contaminacion
    ui->doubleSpinBoxP->setValue(10.0);
}



void RImpulsivo::cambiarImagen() {
    //inicializar valores
    imagenAux =  imagenOriginal;
    srand(time(NULL)); //reinicia semilla aleatoria

    double p = ui->doubleSpinBoxP->value(); //porcentaje de contaminacion
    int N = (int) ((p / 100.0) * (double) imagenOriginal.size()); //numero de pixeles ruidosos a generar
    F = (int) (((double) N / 2.0) + 0.5); //frecuencia absoluta para cada valor del intervalo (redondeado al int mas cercano)

    //inicializar matriz ruido
    for (int i = 0; i < imagenOriginal.width(); i++)
        for (int j = 0; j < imagenOriginal.height(); j++)
            matRuido[i][j] = NO_RUIDO; //valor de NO RUIDO

    //poner ruido en casillas aleatorias de matriz ruido
    int x, y;
    int valor = -255; //primera iteracion
    for (int i = 0; i < 2; i++) { //repite dos veces
        for (int j = 0; j < F; j++) { //F veces por nivel
            do { //encontrar una casilla aleatoria aun sin ruido
                x = rand() % (imagenOriginal.width() - 1); //0..width-1
                y = rand() % (imagenOriginal.height() - 1); //0..width-1
            }
            while (matRuido[x][y] != NO_RUIDO); //con el do..while, obliga a que se ejecuta al menos una vez (como el repeat)

            matRuido[x][y] = valor; //en posicion aleatoria se asigna este valor
        } //for F

        valor = 255; //pasa ahora a segunda iteracion
    } //for 1, 2


    //añadir ruido a imagen
    for (int i = 0; i < imagenAux.width(); i++) //es lo mismo que con imagenOriginal
        for (int j = 0; j < imagenAux.height(); j++) {
            if (matRuido[i][j] != NO_RUIDO) {
                valor = imagenAux.gray(i, j) + matRuido[i][j]; //añadimos ruido
                if (valor > imagenAux.M() - 1)
                    valor = imagenAux.M() - 1;
                else
                    if (valor < 0)
                        valor = 0;
                imagenAux.qimage.setPixel(i, j, valor);
            }
         }

    imagenAux.update();
    imagenRuidoCreada = false; //aun no se ha creado para mostrar
    emit(changed()); //se ha cambiado algo, se lo comunica a aShot para actualizar imageLabel //imageLabel->setPixmap(QPixmap::fromImage(imagenAux.qimage)); //shot->updateImageLabel();//actualiza label
}




void RImpulsivo::crearImagenRuido() { //crea a partir de matriz de ruido. solo sirve para slots verImagenRuido y verHistRuido
    imagenR = imagenOriginal; //solo para las dimensiones

    int valor;
    printf("noruido: %d", NO_RUIDO);
    imagenR.qimage.setColor(NO_RUIDO, qRgb(0, 0, 255));  //el 100 lo reservamos para NO_RUIDO: color azul
    for (int i = 0; i < imagenR.width(); i++) //es lo mismo que con imagenOriginal
        for (int j = 0; j < imagenR.height(); j++) {  
            valor = matRuido[i][j];
            if (valor == -255)
                valor = 0;
            //if valor = 255: es 255
            //if valor == NO_RUIDO: es NO_RUIDO = 100
            imagenR.qimage.setPixel(i, j, valor);
        }

    imagenR.update();
}


void RImpulsivo::aceptar() {
    this->close();
    emit(acepted());
    delete this;
}


void RImpulsivo::cancelar() {
    imagenAux = imagenOriginal; //new Imagen(imagenOriginal->fileName()); //se mantiene una COPIA DE LA IMAGEN, cuidado NO COPIA DE PUNTERO.
    close();
    emit(changed());
    delete this;
}


void RImpulsivo::verHistRuido() {
    if (!imagenRuidoCreada) {
        crearImagenRuido();
        imagenRuidoCreada = true;
    }

      //if (imagenR.max_vin() == imagenR.M() - 1) //si hay mucho NO_RUIDO, bajamos la escala en el eje y
    Histograma *   histograma = new Histograma(0, imagenR); //cambiamos el rango de grises: 0..254, xq el 255 lo reservamos para NO_RUIDO

    histograma->show();
    histograma->move(1100, 500);
}


void RImpulsivo::verImagenRuido() {
    if (!imagenRuidoCreada) {
        crearImagenRuido();
        imagenRuidoCreada = true;
    }

    labelRuido.setPixmap(QPixmap::fromImage(imagenR.qimage));
    labelRuido.move(this->x() + 600, this->y());
    labelRuido.show();
}
