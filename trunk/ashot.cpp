#include "ashot.h"
#include "ui_ashot.h"

#include <math.h>
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sstream>
//#include <string>

//using namespace std;



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
    ui->imageLabel->setScaledContents(true); //se escala para rellenar todo el espacio

    ui->scrollArea = new QScrollArea;
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(ui->imageLabel);
    setCentralWidget(ui->scrollArea);

    //setWindowTitle(tr("aShot 1.0")); //se hace tambien con Qt Designer
    resize(600, 500);

    hasImage = false;

    //setAttribute(Qt::WA_OpaquePaintEvent);
    connectActions();
    //abrir(); //para acelerar la depuracion

    //setMouseTracking(false);
}

aShot::~aShot() {
    delete ui;
}


void aShot::updateImageLabel() {
    ui->imageLabel->setPixmap(QPixmap::fromImage(imagen.qimage));
    //ui->imageLabel->hide();
    //ui->imageLabel->resize(QSize(imagen.width(), imagen.height()));
    //ui->imageLabel->show();
    //adjustScrollBar(ui->scrollArea->horizontalScrollBar(), 1.0);
    //adjustScrollBar(ui->scrollArea->verticalScrollBar(), 1.0);
    scaleFactor = 1.0;

    if (!ui->actionAjustar_a_ventana->isChecked())
        ui->imageLabel->adjustSize();
}


void aShot::pegarImagenRect() {
    imagen.pegarImagen(imagenRect, p1);
}



//protected: events
void aShot::mousePressEvent(QMouseEvent* event) {
    if (hasImage) {
        //printf("(%d, %d)", ui->scrollArea->pos().x(), ui->scrollArea->pos().y());
        int x = event->x() - ui->scrollArea->pos().x(); //tambien event->pos().x()
        int y = event->y() - ui->scrollArea->pos().y();
        if ((x <= ui->imageLabel->width()) && (y <= ui->imageLabel->height()) && (x >= 0) && (y >= 0)) {
            ui->statusBar->showMessage((const QString) "(" + QString::number(x) + ", " + QString::number(y) + ") = " + QString::number(imagen.perfil(x, y)));
            p1 = QPoint(x, y);
        }
    }
}

void aShot::mouseMoveEvent(QMouseEvent *event) {
    if (hasImage) {
        //printf("(%d, %d)", ui->scrollArea->pos().x(), ui->scrollArea->pos().y());
        int x = event->x() - ui->scrollArea->pos().x(); //tambien event->pos().x()
        int y = event->y() - ui->scrollArea->pos().y();
        if ((x <= ui->imageLabel->width()) && (y <= ui->imageLabel->height()) && (x >= 0) && (y >= 0)) {
            ui->statusBar->showMessage((const QString) "(" + QString::number(x) + ", " + QString::number(y) + ") = " + QString::number(imagen.perfil(x, y))); //tambien: event->x()
            p2 = QPoint(x, y);
            update(); //update widget. tambien: repaint()
        }
    }
}


void aShot::mouseReleaseEvent(QMouseEvent *) {
    if (hasImage) {
        //printf("se ha soltado raton");
        imagenRect.setQImage(imagen.qimage.copy(QRect(p1, p2))); //copia imagen a partir de region de seleccion
    }
}


void aShot::mouseDoubleClickEvent(QMouseEvent *) {
    if (hasImage) {
        updateImageLabel(); //quita el rectangulo
        p1 = QPoint(0, 0); //la region de seleccion es toda la imagen
        p2 = QPoint(imagen.width() - 1, imagen.height() - 1);
        imagenRect = imagen;
    }
}


void aShot::paintEvent(QPaintEvent *) {
    if ((hasImage) && (p1 != QPoint(0, 0)) && (p2 != QPoint(imagen.width(), imagen.height()))) { //para no dibujar rect de toda la imagen
        updateImageLabel();
        QPixmap pic = (QPixmap) *ui->imageLabel->pixmap();
        QPainter painter(&pic);
        QPen pen = QPen(Qt::blue, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin); //tambien: pen.setBrush(Qt::blue);
        painter.setPen(pen);

        painter.drawRect(QRect(p1, p2));

        painter.end();
        ui->imageLabel->setPixmap(pic);
    }

}




//slots:

void aShot::updateAll() { //actualiza imagen y imageLabel
    pegarImagenRect();
    updateImageLabel();
}

void aShot::abrirNew() {
    if (!this->hasImage)
        abrir();
    else {
        this->a = new aShot();
        this->a->show();
        this->a->abrir();
    }
}

void aShot::cerrarTodo() {
    qApp->closeAllWindows();
    ui->imageLabel->setPixmap(NULL);
    this->show();
    //imagen = nada, no hace falta
    hasImage = false;
}

void aShot::abrir() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QDir::currentPath()); //"/home/alex/Escritorio/patricia-conde6.jpg";

    if (!fileName.isEmpty()) {
        imagen = Imagen(fileName);
        if (imagen.isNull()) {
            QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
            return;
        }

        resize(imagen.width(), imagen.height() + 200); //+ las barras etc
        enableActions();
        updateZoomActions();
        updateImageLabel();
        hasImage = true;
        p1 = QPoint(0, 0); //la region de seleccion es toda la imagen
        p2 = QPoint(imagen.width() - 1, imagen.height() - 1);
        imagenRect = imagen; //por ahora la imagen de la seleccion es toda la imagen
    }
}



void aShot::guardar() {
    const QString fileName = imagen.fileName();
    imagen.qimage.save(fileName, 0, -1);
}


void aShot::guardarComo() {
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), QDir::currentPath());
    imagen.qimage.save(fileName, 0, -1);
    /* QMessageBox::information(this, tr("aShot Guardar"), tr("No se puede Guardar %1.").arg(fileName));
        return;
    }*/
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
    /*if (imagen == NULL)
        QMessageBox::information(this, tr("Informacion de la imagen"), tr("No se ha podido cargar la imagen"));
    else*/

    if (imagen.isNull())
        QMessageBox::information(this, tr("Informacion de la imagen"), tr("No se ha podido cargar la imagen"));
    else {
        //itoa(image->height(), buffer, 10);
        //QRgb pix = imagen.qimage.pixel(4, 5);
        //int val = qGray(pix); //Returns a gray value (0 to 255) from the given ARGB quadruplet rgb. Formula: (R * 11 + G * 16 + B * 5)/32;
        //tambien esta qGreen, qRed, qBlue
        /*ostringstream s;
        s << imagen.entropia();
        string cadena = s.str();*/

        QMessageBox::information(this, tr("Informacion de la imagen"),
                                 "<p><b>Nombre del fichero: </b>" + imagenRect.fileName() + "</p>"
                                 + "<p><b>Ruta:</b> " + imagenRect.path() + "</p>"
                                 + "<p><b>Extension:</b> " + imagenRect.extension() + "</p>"
                                 + "<p><b>Tamano:</b> " + QString::number((double) imagenRect.qimage.byteCount() / 1024.0) + " KB</p>"
                                 + "<p><b>Error:</b> " + QString::number(imagenRect.error()) + "</p>"
                                 + "<p><b>Dimension en pixeles:</b> " + QString::number(imagenRect.width()) + " x " + QString::number(imagenRect.height()) + " pixeles</p>"
                                 + "<p><b>Numero de pixeles:</b> " + QString::number(imagenRect.size()) + " pixeles</p>"
                                 + "<p><b>Formato:</b> " + imagenRect.formato() + "</p>"
                                 + "<p><b>Brillo:</b> " + QString::number(imagenRect.brillo()) + "</p>"
                                 + "<p><b>Contraste:</b> " + QString::number(imagenRect.contraste()) + "</p>"
                                 +"<p><b>Entropia:</b> " + QString::number(imagenRect.entropia()) + "</p>"
                                 + "<p><b>Rango Dinamico:</b> [" + QString::number(imagenRect.minRango()) + ".." + QString::number(imagenRect.maxRango()) + "]</p>"
                                 + "<p><b>Frecuencia maxima:</b> " + QString::number(imagenRect.maxh()) + " (nivel: " + QString::number(imagenRect.max_vin()) + ")</p>" );

    }

}


void aShot::acercade() {
    QMessageBox::about(this, tr("About aShot 1.0"),
            tr("<p>The <b>aShot 1.0</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}



void aShot::toGray() {
    //QVector<QRgb> tabla = QVector<QRgb>();
    //tabla.append(pix);
    //pix = imagen.qimage.color(indice); //devuelve rgb del indice de tabla indexada
    //indice = imagen.qimage.pixelIndex(i, j); //devuelve el indice de la tabla indexada

    QRgb pix;
    int value;
    for (int i = 0; i < imagen.width(); i++) // igual: qimage.width()
        for (int j = 0; j < imagen.height(); j++) {
            pix = imagen.qimage.pixel(i, j);
            value = (int) (0.299 * (double) qRed(pix) + 0.587 * (double) qGreen(pix) + 0.114 * (double) qBlue(pix)); //NTSC
            //value = qGray(pix); //es casi equivalente
            if (value > imagen.M() - 1)
                value = imagen.M() - 1;

            if (value < 0)
                value = 0;

            pix = qRgb(value, value, value);
            imagen.qimage.setPixel(i, j, pix); //escritura tipo imagen RGB, diferente de 8 bits indexado
        }

    imagen.qimage = imagen.qimage.convertToFormat(QImage::Format_Indexed8); //3: escala de grises indexado

    imagen.update();
    updateImageLabel();
    ui->actionToGray->setEnabled(false);
}


void aShot::negativo() {
    imagenRect.qimage.invertPixels(); //el negativo de la imagen. tambien se podria con un bucle con imagen.negativo(vin)

    imagenRect.update();
    updateAll();
}


//slots de tratamiento para ventanitas de edicion de imagen
void aShot::showNewHistograma() {
    histograma = new Histograma(0, imagenRect);
    histograma->show();
}


void aShot::showNewPerfil() {
    perfil = new Perfil(0, imagenRect);
    perfil->show();
}


void aShot::showNewBC() { //crea bc y lo muestra
    bc = new BrilloContraste(0, imagenRect);
    //this->bc = new BrilloContraste(this); //esto se hara una vez abierta la imagen
    //connect(ui->actionBrillo_Contraste, SIGNAL(triggered()), this->bc, SLOT(show()));
    connect(bc, SIGNAL(changed()), this, SLOT(applyBC()));
    bc->show();
}


/*void aShot::applyDestroyBC() {
    //delete imagen;
    imagenRect = bc->imagenAux; //tambien pasa si se cancela, pero no importa, en ese caso imageAux seria la imagen original
    //imagen = new Imagen(bc->imagenAux->fileName());
    delete bc;
}*/

void aShot::applyBC() {
    imagenRect = bc->imagenAux;
    updateAll();
}


void aShot::showNewLogexp() {
    logexp = new Logexp(0, imagenRect);
    connect(logexp, SIGNAL(changed()), this, SLOT(applyLogexp()));
    logexp->show();
}

void aShot::applyLogexp() {
    imagenRect = logexp->imagenAux;
    updateAll();
}


void aShot::showNewTramos() {
    tramos = new Tramos(0, imagenRect);
    connect(tramos, SIGNAL(closed()), this, SLOT(applyDestroyTramos()));
    tramos->show();
}

void aShot::applyDestroyTramos() {
    imagenRect = tramos->imagenAux;
    updateAll();
    //delete tramos;
}


void aShot::showNewHespecif() {
    hespecif = new Hespecif(0, imagenRect);
    connect(hespecif, SIGNAL(changed()), this, SLOT(applyHespecif()));
    hespecif->show();
}

void aShot::applyHespecif() {
    //printf("eyy cambio hespecif");
    imagenRect = hespecif->imagenAux;
    updateAll();
}


void aShot::showNewDigit() {
    digit = new Digitalizar(0, imagen);
    connect(digit, SIGNAL(changed()), this, SLOT(applyDigit()));
    digit->show();
}

void aShot::applyDigit() {
    imagen = digit->imagenAux;
    imagenRect = imagen;
    updateAll();
}



void aShot::prueba() {
    if (imagen.qimage.format() != 3) { //8-bit indexado, monocromo
        QMessageBox::information(this, tr("Informacion de la imagen"), "no es monocromo indexado");
        return;
    }

    /*QRgb value;
    value = qRgb(122, 163, 39); // 0xff7aa327
    imagen.qimage.setColor(0, value);

    value = qRgb(237, 187, 51); // 0xffedba31
    imagen.qimage.setColor(1, value);

    //value = qRgb(189, 149, 39); // 0xffbd9527
    //image.setColor(2, value);

    for (int i = 0; i < imagen.width(); i++)
        for (int j = 0; j < imagen.height(); j++) {
            if (i == j)
                imagen.qimage.setPixel(i, j, 0);
            else
                imagen.qimage.setPixel(i, j, 1);
        }
    */

    int ntramos = 3;
    double * A = new double[ntramos];
    double * B = new double [ntramos];
    int * extremos = new int [ntramos]; //seria ntramos -1, pero se queda uno mas para q no se qede size 0 en caso de q ntramos=1
    int tramo_i = 0; //tramo actual
    //tambien: Qimage.setColorTable(QVector<QRgb>)
    int * vout = new int [imagen.M()]; //tabla LUT

    //intro datos
    A[0] = 3; B[0] = 2;
    A[1] = 3; B[1] = 0;
    A[2] = 0.3; B[2] = -1;
    extremos[0] = 20; //nivel 20
    extremos[1] = 180;
    //fin intro datos
    for (int vin = 0; vin < imagen.M(); vin++) {

        if (ntramos > 1)
            if (vin == extremos[tramo_i])
                tramo_i++; //siguiente tramo

        vout[vin] = (int) (A[tramo_i] * (double) vin + B[tramo_i]);
        if (vout[vin] > imagen.M() - 1) //en caso de que se haya salido del rango de niveles (0..255)
            vout[vin] = imagen.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }

     imagen.transformar(vout);
     updateImageLabel();  //actualiza label

 }



void aShot::ecualizar() { //ecualizacion del histograma: aproximacion de distribucion uniforme, mantiene entropia

    int * vout = new int [imagenRect.M()]; //tabla LUT
    for (int vin = 0; vin < imagenRect.M(); vin++) {
        vout[vin] = (int) (imagenRect.hAcum(vin) * ((double) imagenRect.M() / (double) imagenRect.size()));
        vout[vin] = (int) (floor((double) vout[vin]) - 1.0);
        if (vout[vin] < 0)
            vout[vin] = 0; //vout = max(0, expresion anterior);

        if (vout[vin] > imagenRect.M() - 1)
            vout[vin] = imagenRect.M() - 1;
        else
            if (vout[vin] < 0)
                vout[vin] = 0;
    }

    imagenRect.transformar(vout);
    pegarImagenRect();
    updateImageLabel();//actualiza label
}


//funciones privadas:

void aShot::connectActions() {   //conecta acciones. las que haga falta una imagen abierta empiezan desactivadas
    connect(ui->action_Salir, SIGNAL(triggered()), qApp, SLOT(quit())); //es igual que ponerlo en el Signal/Slot Editor
    //connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(createNewWindow()));
    connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(abrirNew()));
    connect(ui->action_Cerrar, SIGNAL(triggered()), this, SLOT(close()));
    ui->action_Cerrar->setEnabled(false);
    connect(ui->actionCerrar_todo, SIGNAL(triggered()), this, SLOT(cerrarTodo()));
    ui->actionCerrar_todo->setEnabled(false);

    connect(ui->actionAcerca_de, SIGNAL(triggered()), this, SLOT(acercade()));
    this->ayuda = new Ayuda();
    connect(ui->actionAyuda_de_aShot, SIGNAL(triggered()), this->ayuda, SLOT(show()));
    ui->menuAjustes->setEnabled(false);

    //transformaciones
    connect(ui->actionBrillo_Contraste, SIGNAL(triggered()), this, SLOT(showNewBC()));
    ui->actionBrillo_Contraste->setEnabled(false);
    connect(ui->actionLog_Exp, SIGNAL(triggered()), this, SLOT(showNewLogexp()));
    ui->actionLog_Exp->setEnabled(false);
    connect(ui->actionTramos, SIGNAL(triggered()), this, SLOT(showNewTramos()));
    ui->actionTramos->setEnabled(false);
    connect(ui->actionHespecif, SIGNAL(triggered()), this, SLOT(showNewHespecif()));
    ui->actionHespecif->setEnabled(false);
    connect(ui->actionDigitalizar, SIGNAL(triggered()), this, SLOT(showNewDigit()));
    ui->actionDigitalizar->setEnabled(false);

    connect(ui->actionAjustar_a_ventana, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    ui->actionAjustar_a_ventana->setEnabled(false);
    //ui->actionAjustar_a_ventana->setCheckable(true); //ya se pone en el Qt Designer

    connect(ui->action_Guardar, SIGNAL(triggered()), this, SLOT(guardar()));
    ui->action_Guardar->setEnabled(false);
    connect(ui->actionGuardar_como, SIGNAL(triggered()), this, SLOT(guardarComo()));
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

    connect(ui->actionToGray, SIGNAL(triggered()), this, SLOT(toGray()));
    ui->actionToGray->setEnabled(false);

    connect(ui->actionNegativo, SIGNAL(triggered()), this, SLOT(negativo()));
    ui->actionNegativo->setEnabled(false);

    connect(ui->actionEcualizar, SIGNAL(triggered()), this, SLOT(ecualizar()));
    ui->actionEcualizar->setEnabled(false);

    connect(ui->action_Histograma, SIGNAL(triggered()), this, SLOT(showNewHistograma()));
    ui->action_Histograma->setEnabled(false);

    connect(ui->actionPerfil, SIGNAL(triggered()), this, SLOT(showNewPerfil()));
    ui->actionPerfil->setEnabled(false);

    connect(ui->actionPrueba, SIGNAL(triggered()), this, SLOT(prueba())); //prueba: cuidado no se ha hecho el enable false y luego true

}

void aShot::enableActions() {
    ui->menuAjustes->setEnabled(true);
    ui->actionBrillo_Contraste->setEnabled(true);
    ui->actionLog_Exp->setEnabled(true);
    ui->actionTramos->setEnabled(true);
    ui->actionHespecif->setEnabled(true);
    ui->actionDigitalizar->setEnabled(true);

    ui->actionAjustar_a_ventana->setEnabled(true);
    ui->action_Cerrar->setEnabled(true);
    ui->actionCerrar_todo->setEnabled(true);
    ui->action_Guardar->setEnabled(true);
    ui->actionGuardar_como->setEnabled(true);
    ui->actionIm_primir->setEnabled(true);
    ui->action_Informacion_imagen->setEnabled(true);
    ui->actionNegativo->setEnabled(true);
    ui->actionEcualizar->setEnabled(true);
    ui->action_Histograma->setEnabled(true);
    ui->actionPerfil->setEnabled(true);

    if (imagen.qimage.format() != 3) //si no es en escala de grises se habilita
        ui->actionToGray->setEnabled(true);
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



