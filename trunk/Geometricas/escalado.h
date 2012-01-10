#ifndef ESCALADO_H
#define ESCALADO_H

#include <QWidget>
#include "imagen.h"

#define MAX_MULTP 10 //maximo de veces que se multiplica las dimensiones originales

#define VMP 0  //indices de ui->comboxBoxInterpolacion
#define BILINEAL 1

namespace Ui {
    class Escalado;
}

class Escalado : public QWidget
{
    Q_OBJECT

public:
    explicit Escalado(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Escalado();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::Escalado *ui;

    Imagen imagenOrig;
    int anchura, altura; //nuevas dimensiones


public slots:
    void cambiarImagen();
    void aceptar();
    void cancelar();

    void setDimensiones(); //actualiza valores segun spinBox de dimensiones
    void setPDimensiones(); //actualiza valores segun dSpinBox de porcentajes

signals:
    void changed();
    void acepted();
};

#endif // ESCALADO_H
