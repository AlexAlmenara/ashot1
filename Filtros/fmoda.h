#ifndef FMODA_H
#define FMODA_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FModa;
}

class FModa : public QWidget
{
    Q_OBJECT

public:
    explicit FModa(QWidget *parent = 0, Imagen image = Imagen());
    ~FModa();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FModa *ui;

    Imagen imagenOriginal;

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FMODA_H
