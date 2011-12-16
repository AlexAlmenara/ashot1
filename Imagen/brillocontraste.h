#ifndef BRILLOCONTRASTE_H
#define BRILLOCONTRASTE_H

#include <QWidget>
#include "imagen.h"
//#include "ashot.h"

//class aShot; //dado que es una dependencia ciclica, se pone asi e indica que ya se definira mas tarde

namespace Ui {
    class BrilloContraste;
}

class BrilloContraste : public QWidget
{
    Q_OBJECT

public:
    explicit BrilloContraste(QWidget *parent = 0, Imagen imagen = Imagen());
    ~BrilloContraste();

    Imagen imagenOriginal;
    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios que se hagan al cambiar valores de brillo y contraste
    //QLabel * imageLabel;
    //aShot * shot; //solo coge el puntero de la apliacion principal para poder modificar la imagelabel y su imagen


public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();
    void reiniciar();

private:
    Ui::BrilloContraste *ui;

    void setValues(Imagen imagen);


signals:
    void updateSliderB(int);
    void updateSpinBoxB(double);

    void updateSliderC(int);
    void updateSpinBoxC(double);
    //void closed();
    void changed();

private slots:
    void intToDoubleB(int value);
    void DoubleToIntB(double value);
    void intToDoubleC(int value);
    void DoubleToIntC(double value);

};

#endif // BRILLOCONTRASTE_H
