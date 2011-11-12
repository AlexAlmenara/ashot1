#include "ashot.h"
#include "ui_ashot.h"

//#include <stdio.h>
//#include <stdlib.h>



aShot::aShot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::aShot)
{
    ui->setupUi(this);

    //PRUEBAS:
    //BrilloContraste bc;
    //bc.show();
    //ui->mdiArea->addSubWindow();
    //ui->subwindow->hide();
    //connect(ui->action_Abrir, SIGNAL(triggered()), ui->subwindow, SLOT(close()));
    //connect(ui->action_Abrir, SIGNAL(triggered()), bc, SLOT(show()));
    //QMainWindow qm = new QMainWindow(this);
    //qm.show();
    //Form f; //QWidget
    //connect(ui->action_Abrir, SIGNAL(triggered()), f, SLOT(show()));
    //connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(OpenNewWindow()));
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);

    ui->scrollArea = new QScrollArea;
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(ui->imageLabel);
    setCentralWidget(ui->scrollArea);

    //setWindowTitle(tr("aShot 1.0")); //se hace tambien con Qt Designer
    resize(500, 400);

    connectActions();
    abrir(); //para acelerar la depuracion

}

aShot::~aShot()
{
    delete ui;
}



//slots:

void aShot::createNewWindow() {
    this->a = new aShot();
    this->a->show();
}

void aShot::cerrarTodo() {
    qApp->closeAllWindows();
    ui->imageLabel->setPixmap(NULL);
    this->show();
}

void aShot::abrir() {
    QString fileName = "/home/alex/Escritorio/huevo.tif";  //QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

    if (!fileName.isEmpty()) {
        image = new QImage(fileName);
        if (image->isNull()) {
            QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
            return;
        }

        ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
        scaleFactor = 1.0;

        enableActions();
        updateZoomActions();

        if (!ui->actionAjustar_a_ventana->isChecked())
            ui->imageLabel->adjustSize();
    }
}

void aShot::imprimir() {
    Q_ASSERT(ui->imageLabel->pixmap());//comprueba que haya imagen
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = ui->imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(ui->imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *ui->imageLabel->pixmap());
    }
#endif
}

void aShot::zoomIn() {
    scaleImage(1.25);
}

void aShot::zoomOut() {
    scaleImage(0.8);
}

void aShot::normalSize() {
    ui->imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void aShot::fitToWindow() {
    bool fitToWindow = ui->actionAjustar_a_ventana->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateZoomActions();
}


void aShot::info_imagen() {
    if (image != NULL)
        if (image->isNull())
            QMessageBox::information(this, tr("Informacion de la imagen"), tr("No se ha podido cargar la imagen"));
        else {
            //itoa(image->height(), buffer, 10);
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


            //size = width * height
            QRgb pix = image->pixel(4, 5);
            int val = qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;
            QMessageBox::information(this, tr("Informacion de la imagen"),
                                     "height: " + QString::number(image->height()) + ", width: " + QString::number(image->width()) + "\nFormato: " + format
                                     + "val: " + QString::number(val));

        }

}




void aShot::prueba() {
    if (image->format() != 3) { //8-bit indexado, monocromo
        QMessageBox::information(this, tr("Informacion de la imagen"), "no es monocromo indexado");
        return;
    }

    QRgb value;
    value = qRgb(122, 163, 39); // 0xff7aa327
    image->setColor(0, value);

    value = qRgb(237, 187, 51); // 0xffedba31
    image->setColor(1, value);

    //value = qRgb(189, 149, 39); // 0xffbd9527
    //image.setColor(2, value);

    for (int i = 0; i < image->width(); i++)
        for (int j = 0; j < image->height(); j++) {
            if (i == j)
                image->setPixel(i, j, 0);
            else
                image->setPixel(i, j, 1);
        }


    //actualiza label
    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
    scaleFactor = 1.0;

 }


//funciones privadas:

void aShot::connectActions() {   //conecta acciones. las que haga falta una imagen abierta empiezan desactivadas
    connect(ui->action_Salir, SIGNAL(triggered()), qApp, SLOT(quit())); //es igual que ponerlo en el Signal/Slot Editor
    //connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(createNewWindow()));
    connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(abrir()));
    connect(ui->action_Cerrar, SIGNAL(triggered()), this, SLOT(close()));
    ui->action_Cerrar->setEnabled(false);
    connect(ui->actionCerrar_todo, SIGNAL(triggered()), this, SLOT(cerrarTodo()));
    ui->actionCerrar_todo->setEnabled(false);

    this->acercade = new Acercade();
    connect(ui->actionAcerca_de, SIGNAL(triggered()), this->acercade, SLOT(show()));
    this->ayuda = new Ayuda();
    connect(ui->actionAyuda_de_aShot, SIGNAL(triggered()), this->ayuda, SLOT(show()));
    ui->menuAjustes->setEnabled(false);
    this->bc = new BrilloContraste();
    connect(ui->actionBrillo_Contraste, SIGNAL(triggered()), this->bc, SLOT(show()));
    ui->actionBrillo_Contraste->setEnabled(false);

    connect(ui->actionAjustar_a_ventana, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    ui->actionAjustar_a_ventana->setEnabled(false);
    //ui->actionAjustar_a_ventana->setCheckable(true); //ya se pone en el Qt Designer

    connect(ui->action_Guardar, SIGNAL(triggered()), this, SLOT(prueba()));
    ui->action_Guardar->setEnabled(false);
    ui->actionGuardar_como->setEnabled(false);

    connect(ui->actionIm_primir, SIGNAL(triggered()), this, SLOT(imprimir()));
    ui->actionIm_primir->setEnabled(false);

    connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomIn()));
    ui->actionZoom_In->setEnabled(false);

    connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOut()));
    ui->actionZoom_Out->setEnabled(false);
    connect(ui->actionTama_o_normal, SIGNAL(triggered()), this, SLOT(normalSize()));
    ui->actionTama_o_normal->setEnabled(false);

    connect(ui->action_Informacion_imagen, SIGNAL(triggered()), this, SLOT(info_imagen()));
    ui->action_Informacion_imagen->setEnabled(false);

}

void aShot::enableActions() {
    ui->menuAjustes->setEnabled(true);
    ui->actionBrillo_Contraste->setEnabled(true);
    ui->actionAjustar_a_ventana->setEnabled(true);
    ui->action_Cerrar->setEnabled(true);
    ui->actionCerrar_todo->setEnabled(true);
    ui->action_Guardar->setEnabled(true);
    ui->actionGuardar_como->setEnabled(true);
    ui->actionIm_primir->setEnabled(true);
    ui->action_Informacion_imagen->setEnabled(true);
}

void aShot::updateZoomActions() { //activa o desactiva acciones de zoom segun la accion fitTowindow
    ui->actionZoom_In->setEnabled(!ui->actionAjustar_a_ventana->isChecked());
    ui->actionZoom_Out->setEnabled(!ui->actionAjustar_a_ventana->isChecked());
    ui->actionTama_o_normal->setEnabled(!ui->actionAjustar_a_ventana->isChecked());
}


void aShot::scaleImage(double factor) {
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;
    ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    ui->actionZoom_In->setEnabled(scaleFactor < 3.0);
    ui->actionZoom_Out->setEnabled(scaleFactor > 0.333);
}

void aShot::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

