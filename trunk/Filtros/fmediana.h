#ifndef FMEDIANA_H
#define FMEDIANA_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FMediana;
}

class FMediana : public QWidget
{
    Q_OBJECT

public:
    explicit FMediana(QWidget *parent = 0, Imagen image = Imagen());
    ~FMediana();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FMediana *ui;

    Imagen imagenOriginal;

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FMEDIANA_H
