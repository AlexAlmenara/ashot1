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
    undo = QStack<Imagen>();
    redo = QStack<Imagen>();

    //setAttribute(Qt::WA_OpaquePaintEvent);
    connectActions();
    enableActions(false);
    abrir(); //para acelerar la depuracion

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



void aShot::updateAll() { //actualiza imagen y imageLabel
    pegarImagenRect();
    updateImageLabel();
}



void aShot::addDeshacer() { //se añade tras un cambio en la imagen
    undo.push(imagenAnt);
    ui->actionDeshacer->setEnabled(true);

    ui->actionAnterior_histograma->setEnabled(true);
    ui->actionAnterior_imagen->setEnabled(true);
    redo.clear(); //ya no hay nada que rehacer
    ui->actionRehacer->setEnabled(false);
}


//protected: events
void aShot::mousePressEvent(QMouseEvent* event) {
    if (hasImage) {
        //printf("(%d, %d)", ui->scrollArea->pos().x(), ui->scrollArea->pos().y());
        int x = event->x() - ui->scrollArea->pos().x(); //tambien event->pos().x()
        int y = event->y() - ui->scrollArea->pos().y();
        if ((x <= ui->imageLabel->width()) && (y <= ui->imageLabel->height()) && (x >= 0) && (y >= 0)) {
            ui->statusBar->showMessage((const QString) "(" + QString::number(x) + ", " + QString::number(y) + ") = " + QString::number(imagen.gray(x, y)));
            p1 = QPoint(x, y);
        }
    }
}

void aShot::mouseMoveEvent(QMouseEvent *event) {
    if (hasImage) {
        //printf("(%d, %d)", ui->scrollArea->pos().x(), ui->scrollArea->pos().y());
        int x = event->x() - ui->scrollArea->pos().x(); //tambien event->pos().x()
        int y = event->y() - ui->scrollArea->pos().y();
        if ((x < ui->imageLabel->width()) && (y < ui->imageLabel->height()) && (x >= 0) && (y >= 0)) {
            ui->statusBar->showMessage((const QString) "(" + QString::number(x) + ", " + QString::number(y) + ") = " + QString::number(imagen.gray(x, y))); //tambien: event->x()
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
    //ui->imageLabel = new QLabel();
    enableActions(false);
    undo.clear();
    redo.clear();

    this->show();
    //imagen = nada, no hace falta
    hasImage = false;
}

void aShot::abrir() {
    QString fileName = "/home/alex/Escritorio/berserk.jpg"; //QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QDir::currentPath()); //

    if (!fileName.isEmpty()) {
        imagen = Imagen(fileName);
        if (imagen.isNull()) {
            QMessageBox::information(this, tr("aShot Abrir"), tr("No se puede cargar %1.").arg(fileName));
            return;
        }

        resize(imagen.width(), imagen.height() + 200); //+ las barras etc
        enableActions(true);
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



void aShot::deshacer() {
  //if (undo.size > 0)
  redo.push(imagen); //guarda por si nos equivocamos con deshacer y queremos volver
  ui->actionRehacer->setEnabled(true);

  imagen = undo.pop(); //quita de la pila y lo devuelve
  imagenRect = imagen;
  if (undo.size() == 0) { //ya no hay mas que deshacer
      ui->actionDeshacer->setEnabled(false);
      ui->actionAnterior_histograma->setEnabled(false);
      ui->actionAnterior_imagen->setEnabled(false);
   }

  updateAll();

}

void aShot::rehacer() {
    //if (redo.size() > 0)
    //addDeshacer(); //es lo mismo que:
    undo.push(imagen);
    ui->actionDeshacer->setEnabled(true); //por si se hizo todos los deshacer posibles
    ui->actionAnterior_histograma->setEnabled(true);
    ui->actionAnterior_imagen->setEnabled(true);

    imagen = redo.pop(); //quita de la pila y lo devuelve
    imagenRect = imagen;
    if (redo.size() == 0) //ya no hay mas que deshacer
        ui->actionRehacer->setEnabled(false);

    updateAll();
}


void aShot::anteriorImagen() { //solo se ejecutara si undo tiene algo
    labelAnt.setPixmap(QPixmap::fromImage(undo.top().qimage));
    labelAnt.show();
    labelAnt.move(this->x() + 600, this->y());
}



void aShot::anteriorHistograma() { //solo se ejecutara si undo tiene algo
    Histograma * hist = new Histograma(0, undo.top());
    hist->show();
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
                                 + "<p><b>Moda: </b> " + QString::number(imagenRect.moda()) + ", correspondiente a la <b>frecuencia maxima: </b> " + QString::number(imagenRect.maxh()) + "</p>"
                                 + "<p><b>Mediana: </b>" + QString::number(imagenRect.mediana()) + "</p>");

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
    QVector<QRgb> tabla = QVector<QRgb>(256);
    //tabla.append(pix);
    //pix = imagen.qimage.color(indice); //devuelve rgb del indice de tabla indexada
    //indice = imagen.qimage.pixelIndex(i, j); //devuelve el indice de la tabla indexada


    /*imagen.qimage = imagen.qimage.convertToFormat(QImage::Format_Indexed8); //3: escala de grises indexad

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
            imagen.qimage.setColor(value, pix);
            imagen.qimage.setPixel(i, j, value); //escritura tipo imagen RGB, diferente de 8 bits indexado
        }*/

    imagenAnt = imagen;
    addDeshacer();


    QRgb pix;
    int value;
    for (int i = 0; i < imagen.width(); i++) // igual: qimage.width()
        for (int j = 0; j < imagen.height(); j++) {
            pix = imagen.qimage.pixel(i, j);
            value = (int) ((0.299 * (double) qRed(pix) + 0.587 * (double) qGreen(pix) + 0.114 * (double) qBlue(pix)) + 0.5); //NTSC
            //value = qGray(pix); //es casi equivalente
            if (value > imagen.M() - 1)
                value = imagen.M() - 1;

            if (value < 0)
                value = 0;

            pix = qRgb(value, value, value);
            tabla.remove(value);
            tabla.insert(value, pix);
            imagen.qimage.setPixel(i, j, pix); //escritura tipo imagen RGB, diferente de 8 bits indexado
        }

    /*for (int i = 0; i < tabla.size(); i++)
        printf("%d, ", qGray(tabla[i]));*/

    imagen.qimage = imagen.qimage.convertToFormat(QImage::Format_Indexed8, tabla); //3: escala de grises indexado

    /*for (int i = 0; i < imagen.width(); i++) // igual: qimage.width()
        for (int j = 0; j < imagen.height(); j++) {
            pix = imagen.qimage.pixel(i, j);
            value = qGray(pix);
            pix = qRgb(value, value, value);
            imagen.qimage.setColor(value, pix);
            //imagen.qimage.setPixel(i, j, value);
        }*/


    imagen.update();
    updateImageLabel();
    ui->actionToGray->setEnabled(false);
}


void aShot::negativo() {
    imagenAnt = imagen;
    addDeshacer();
    imagenRect.qimage.invertPixels(); //el negativo de la imagen. tambien se podria con un bucle con imagen.negativo(vin)

    imagenRect.update();
    updateAll();
}


//slots de tratamiento para ventanitas de edicion de imagen
void aShot::showNewHistograma() {
    Histograma * histograma = new Histograma(0, imagenRect);
    histograma->show();
}


void aShot::showNewPerfil() {
    Perfil * perfil = new Perfil(0, imagenRect);
    perfil->show();
}


void aShot::showNewBC() { //crea bc y lo muestra
    bc = new BrilloContraste(0, imagenRect);
    imagenAnt = imagen;
    //this->bc = new BrilloContraste(this); //esto se hara una vez abierta la imagen
    //connect(ui->actionBrillo_Contraste, SIGNAL(triggImpulsivoered()), this->bc, SLOT(show()));
    connect(bc, SIGNAL(changed()), this, SLOT(applyBC()));
    connect(bc, SIGNAL(acepted()), this, SLOT(addDeshacer()));
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
    imagenAnt = imagen;
    connect(logexp, SIGNAL(changed()), this, SLOT(applyLogexp()));
    connect(logexp, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    logexp->show();
}

void aShot::applyLogexp() {
    imagenRect = logexp->imagenAux;
    updateAll();
}


void aShot::showNewTramos() {
    tramos = new Tramos(0, imagenRect);
    imagenAnt = imagen;
    connect(tramos, SIGNAL(closed()), this, SLOT(applyDestroyTramos()));
    tramos->show();
}

void aShot::applyDestroyTramos() {
    addDeshacer();
    imagenRect = tramos->imagenAux;
    updateAll();
    //delete tramos;
}


void aShot::showNewHespecif() {
    hespecif = new Hespecif(0, imagenRect);
    imagenAnt = imagen;
    connect(hespecif, SIGNAL(changed()), this, SLOT(applyHespecif()));
    connect(hespecif, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    hespecif->show();
}

void aShot::applyHespecif() {
    //printf("eyy cambio hespecif");
    imagenRect = hespecif->imagenAux;
    updateAll();
}


void aShot::showNewDigit() {
    digit = new Digitalizar(0, imagenRect);
    imagenAnt = imagen;
    connect(digit, SIGNAL(closed()), this, SLOT(applyDigit()));
    digit->show();
}

void aShot::applyDigit() {
    //imagen = digit->imagenAux;
    addDeshacer();
    imagenRect = digit->imagenAux;
    updateAll();
}

void aShot::showNewDiferencia() {
    diferencia = new Diferencia(0, imagenRect);
    imagenAnt = imagen;
    connect(diferencia, SIGNAL(changed()), this, SLOT(applyDiferencia()));
    connect(diferencia, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    diferencia->show();
}

void aShot::applyDiferencia() {
    imagenRect = diferencia->imagenAux;
    updateAll();
}



void aShot::prueba() { //pintar negro en diagonal
    imagenAnt = imagen;
    addDeshacer();

    if (imagen.qimage.format() != 3) { //8-bit indexado, monocromo
        QMessageBox::information(this, tr("Informacion de la imagen"), "no es monocromo indexado");
        return;
    }

    for (int i = 0; i < imagenRect.nDiagonal(); i++) {
        QPoint p = imagenRect.posPerfil(i);
        imagenRect.qimage.setPixel(p.x(), p.y(), 0);
    }

    imagenRect.update();

    printf("\nmoda de toda la region: %d", imagenRect.moda(0, 0, imagenRect.width() - 1, imagenRect.height() - 1));
    printf("\nmoda normal: %d", imagenRect.moda());

    printf("\nmediana de toda la region: %f", imagenRect.mediana(0, 0, imagenRect.width() - 1, imagenRect.height() - 1));
    printf("\nmediana normal: %f", imagenRect.mediana());

    printf("\nbrillo de toda la region: %f", imagenRect.brillo(0, 0, imagenRect.width() - 1, imagenRect.height() - 1));
    printf("\nbrillo normal: %f", imagenRect.brillo());

    printf("\ncontraste de toda la region: %f", imagenRect.contraste(0, 0, imagenRect.width() - 1, imagenRect.height() - 1));
    printf("\ncontraste normal: %f", imagenRect.contraste());

    updateAll();  //actualiza label

}



void aShot::ecualizar() { //ecualizacion del histograma: aproximacion de distribucion uniforme, mantiene entropia
    imagenAnt = imagen;
    addDeshacer();

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
    //connect(this, SIGNAL(destroyed()), qApp, SLOT(quit()));
    connect(ui->action_Salir, SIGNAL(triggered()), qApp, SLOT(quit())); //es igual que ponerlo en el Signal/Slot Editor
    //connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(createNewWindow()));
    connect(ui->action_Abrir, SIGNAL(triggered()), this, SLOT(abrirNew()));
    connect(ui->action_Cerrar, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionCerrar_todo, SIGNAL(triggered()), this, SLOT(cerrarTodo()));

    connect(ui->actionAcerca_de, SIGNAL(triggered()), this, SLOT(acercade()));
    this->ayuda = new Ayuda();
    connect(ui->actionAyuda_de_aShot, SIGNAL(triggered()), this->ayuda, SLOT(show()));

    //transformaciones
    connect(ui->actionBrillo_Contraste, SIGNAL(triggered()), this, SLOT(showNewBC()));
    connect(ui->actionLog_Exp, SIGNAL(triggered()), this, SLOT(showNewLogexp()));
    connect(ui->actionTramos, SIGNAL(triggered()), this, SLOT(showNewTramos()));
    connect(ui->actionHespecif, SIGNAL(triggered()), this, SLOT(showNewHespecif()));
    connect(ui->actionDigitalizar, SIGNAL(triggered()), this, SLOT(showNewDigit()));
    connect(ui->actionDiferencia, SIGNAL(triggered()), this, SLOT(showNewDiferencia()));

    connect(ui->actionAjustar_a_ventana, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    //ui->actionAjustar_a_ventana->setCheckable(true); //ya se pone en el Qt Designer

    connect(ui->action_Guardar, SIGNAL(triggered()), this, SLOT(guardar()));
    connect(ui->actionGuardar_como, SIGNAL(triggered()), this, SLOT(guardarComo()));
    connect(ui->actionIm_primir, SIGNAL(triggered()), this, SLOT(imprimir()));

    connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(ui->actionTama_o_normal, SIGNAL(triggered()), this, SLOT(normalSize()));

    connect(ui->action_Informacion_imagen, SIGNAL(triggered()), this, SLOT(info_imagen()));

    connect(ui->actionToGray, SIGNAL(triggered()), this, SLOT(toGray()));
    connect(ui->actionNegativo, SIGNAL(triggered()), this, SLOT(negativo()));
    connect(ui->actionEcualizar, SIGNAL(triggered()), this, SLOT(ecualizar()));
    connect(ui->action_Histograma, SIGNAL(triggered()), this, SLOT(showNewHistograma()));
    connect(ui->actionPerfil, SIGNAL(triggered()), this, SLOT(showNewPerfil()));

    connect(ui->actionDeshacer, SIGNAL(triggered()), this, SLOT(deshacer()));
    connect(ui->actionRehacer, SIGNAL(triggered()), this, SLOT(rehacer()));
    connect(ui->actionAnterior_imagen, SIGNAL(triggered()), this, SLOT(anteriorImagen()));
    connect(ui->actionAnterior_histograma, SIGNAL(triggered()), this, SLOT(anteriorHistograma()));

    ui->actionDeshacer->setEnabled(false); //empiezan en false hasta algun cambio en imagen
    ui->actionRehacer->setEnabled(false);
    ui->actionAnterior_histograma->setEnabled(false);
    ui->actionAnterior_imagen->setEnabled(false);

    connect(ui->actionPrueba, SIGNAL(triggered()), this, SLOT(prueba())); //prueba: cuidado no se ha hecho el enable false y luego true

    //Practica 2: filtros y ruido
    connect(ui->actionRUniforme, SIGNAL(triggered()), this, SLOT(showNewRUniforme()));
    connect(ui->actionRImpulsivo, SIGNAL(triggered()), this, SLOT(showNewRImpulsivo()));
    connect(ui->actionRGuassiano, SIGNAL(triggered()), this, SLOT(showNewRGaussiano()));
    connect(ui->actionFMedia, SIGNAL(triggered()), this, SLOT(showNewFMedia()));
    connect(ui->actionFModa, SIGNAL(triggered()), this, SLOT(showNewFModa()));
}

void aShot::enableActions(bool b) {
    ui->menuAjustes->setEnabled(b);
    ui->actionBrillo_Contraste->setEnabled(b);
    ui->actionLog_Exp->setEnabled(b);
    ui->actionTramos->setEnabled(b);
    ui->actionHespecif->setEnabled(b);
    ui->actionDigitalizar->setEnabled(b);
    ui->actionDiferencia->setEnabled(b);

    ui->actionAjustar_a_ventana->setEnabled(b);
    ui->action_Cerrar->setEnabled(b);
    ui->actionCerrar_todo->setEnabled(b);
    ui->action_Guardar->setEnabled(b);
    ui->actionGuardar_como->setEnabled(b);
    ui->actionIm_primir->setEnabled(b);
    ui->action_Informacion_imagen->setEnabled(b);
    ui->actionNegativo->setEnabled(b);
    ui->actionEcualizar->setEnabled(b);
    ui->action_Histograma->setEnabled(b);
    ui->actionPerfil->setEnabled(b);

    if (b == false) { //solo si es false, a true ya se hace cuando se haga un cambio en imagen
        ui->actionDeshacer->setEnabled(false);
        ui->actionRehacer->setEnabled(false);
        ui->actionAnterior_histograma->setEnabled(false);
        ui->actionAnterior_imagen->setEnabled(false);
    }

    if ((b == true) && (imagen.qimage.format() != 3)) //si no es en escala de grises se habilita
        ui->actionToGray->setEnabled(true);
    else
        ui->actionToGray->setEnabled(false);

    //Practica 2:
    ui->actionRUniforme->setEnabled(b);
    ui->actionRGuassiano->setEnabled(b);
    ui->actionRImpulsivo->setEnabled(b);

    ui->actionFMedia->setEnabled(b);
    ui->actionFModa->setEnabled(b);
    ui->actionFMediana->setEnabled(b);
    ui->actionFK_Vecinos->setEnabled(b);
    ui->actionFDif_Estadistica->setEnabled(b);
    ui->actionFGuassiano->setEnabled(b);
    ui->actionDefinirFiltro->setEnabled(b);
    ui->actionUsarFiltroDefinido->setEnabled(b);
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



//Practica 2:
void aShot::showNewRUniforme() {
    rUniforme = new RUniforme(0, imagenRect);
    imagenAnt = imagen;
    connect(rUniforme, SIGNAL(changed()), this, SLOT(applyRUniforme()));
    connect(rUniforme, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    rUniforme->show();
}


void aShot::applyRUniforme() {
    imagenRect = rUniforme->imagenAux;
    updateAll();
}


void aShot::showNewRImpulsivo() {
    rImpulsivo = new RImpulsivo(0, imagenRect);
    imagenAnt = imagen;
    connect(rImpulsivo, SIGNAL(changed()), this, SLOT(applyRImpulsivo()));
    connect(rImpulsivo, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    rImpulsivo->show();
}


void aShot::applyRImpulsivo() {
    imagenRect = rImpulsivo->imagenAux;
    updateAll();
}


void aShot::showNewRGaussiano() {
    rGaussiano = new RGaussiano(0, imagenRect);
    imagenAnt = imagen;
    connect(rGaussiano, SIGNAL(changed()), this, SLOT(applyRGaussiano()));
    connect(rGaussiano, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    rGaussiano->show();
}


void aShot::applyRGaussiano() {
    imagenRect = rGaussiano->imagenAux;
    updateAll();
}



void aShot::showNewFMedia() {
    fMedia = new FMedia(0, imagenRect);
    imagenAnt = imagen;
    connect(fMedia, SIGNAL(changed()), this, SLOT(applyFMedia()));
    connect(fMedia, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    fMedia->show();
}


void aShot::applyFMedia() {
    imagenRect = fMedia->imagenAux;
    updateAll();
}



void aShot::showNewFModa() {
    fModa = new FModa(0, imagenRect);
    imagenAnt = imagen;
    connect(fModa, SIGNAL(changed()), this, SLOT(applyFModa()));
    connect(fModa, SIGNAL(acepted()), this, SLOT(addDeshacer()));
    fModa->show();
}


void aShot::applyFModa() {
    imagenRect = fModa->imagenAux;
    updateAll();
}



