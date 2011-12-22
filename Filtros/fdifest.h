#ifndef FDIFEST_H
#define FDIFEST_H

#include <QWidget>
#include "imagen.h"

namespace Ui {
    class FDifest;
}

class FDifest : public QWidget
{
    Q_OBJECT

public:
    explicit FDifest(QWidget *parent = 0, Imagen image = Imagen());
    ~FDifest();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::FDifest *ui;

    Imagen imagenOriginal;

public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();


signals:
    void changed();
    void acepted();
};

#endif // FDIFEST_H
