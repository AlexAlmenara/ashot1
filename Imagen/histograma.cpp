#include "histograma.h"
#include "ui_histograma.h"

Histograma::Histograma(QWidget *parent, Imagen image) :
    QWidget(parent),
    ui(new Ui::Histograma)
{
    ui->setupUi(this);


    this->imagen = image;
    setWindowTitle("Histograma de " + imagen.fileName());

    connect(ui->radioButtonAbs, SIGNAL(clicked()), this, SLOT(histAbs()));
    connect(ui->radioButtonRel, SIGNAL(clicked()), this, SLOT(histRel()));
    connect(ui->radioButtonAcum, SIGNAL(clicked()), this, SLOT(histAcum()));
    connect(ui->radioButtonAcumNorm, SIGNAL(clicked()), this, SLOT(histAcumNorm()));
    ui->radioButtonAbs->setChecked(true); //empieza por defecto histograma absoluto
    //firstPainted = false;
    histAbs();
    //firstPainted = true;


    /*hist = QVector<double>(imagen.M());
    for (int vin = 0; vin < imagen.M(); vin++)
        hist.insert(vin, imagen.hAbs(vin));

    function = new Function(this, hist, 0, imagen.M() - 1, 0, imagen.maxh()); //por defecto histograma absoluto
    function->resize(400, 200);
    function->show();
    function->move(40, 20);
    ui->verticalLayout->addWidget(function);
    connect(function, SIGNAL(moused()), this, SLOT(updatePunto()));*/

}

Histograma::~Histograma()
{
    delete ui;
}


/*void Histograma::paintEvent(QPaintEvent *) {
    function->resize(width - 300, height - 300);
    return;
}*/


void Histograma::updatePunto() {
    ui->labelPunto->setText("Nivel: " + QString::number((int) function->getX()) + ", Valor: " + QString::number(function->getY()));
}


void Histograma::newFunction(double ymax) {
    //if (firstPainted)
        //ui->verticalLayout->removeWidget(function);
        //ui->formLayout->removeWidget(function);
        //ui->gridLayout->removeWidget(function);

    function = new Function(this, hist, 0, imagen.M() - 1, 0, ymax); //por defecto histograma absoluto
    function->resize(400, 200);
    function->show();

    //ui->horizontalLayoutFunction->setGeometry((const QRect) QRect(function->size().width(), function->size().height()));
    //ui->horizontalLayoutFunction->addWidget(function);
    function->move(40, 20);
    //ui->verticalLayout->addWidget(function);
    //ui->formLayout->addWidget(function);
    //ui->gridLayout->addWidget(function);
    connect(function, SIGNAL(moused()), this, SLOT(updatePunto()));
}


void Histograma::histAbs() {
    hist = QVector<double>(imagen.M());
    for (int vin = 0; vin < imagen.M(); vin++)
        hist.insert(vin, imagen.hAbs(vin));

    newFunction(imagen.maxh());
}

void Histograma::histRel() {
    hist = QVector<double>(imagen.M());
    for (int vin = 0; vin < imagen.M(); vin++)
        hist.insert(vin, imagen.hRel(vin));

    newFunction((double)imagen.maxh() / (double) imagen.size());
}


void Histograma::histAcum() {
    hist = QVector<double>(imagen.M());
    for (int vin = 0; vin < imagen.M(); vin++)
        hist.insert(vin, imagen.hAcum(vin));

    newFunction((double) imagen.size());
}


void Histograma::histAcumNorm() {
    hist = QVector<double>(imagen.M());
    for (int vin = 0; vin < imagen.M(); vin++)
        hist.insert(vin, imagen.hAcumNorm(vin));

    newFunction(1.0);
}

