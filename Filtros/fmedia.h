#ifndef FMEDIA_H
#define FMEDIA_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FMedia;
}

class FMedia : public QWidget
{
    Q_OBJECT

public:
    explicit FMedia(QWidget *parent = 0, Imagen image = Imagen());
    ~FMedia();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FMedia *ui;

    Imagen imagenOriginal;

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FMEDIA_H
