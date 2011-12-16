#ifndef TRAMOS_H
#define TRAMOS_H

#include <QWidget>
#include "imagen.h"
#include "function.h"

namespace Ui {
    class Tramos;
}

class Tramos : public QWidget
{
    Q_OBJECT

public:
    explicit Tramos(QWidget *parent = 0, Imagen imagen = Imagen());
    ~Tramos();

    Imagen imagenAux; //imagen auxiliar para volcar en imagelabel de aShot los cambios

private:
    Ui::Tramos *ui;

    int nTramos; //numero de tramos que introduce el usuario
    int puntoi; //punto i actual a introducir por el usuario
    QPoint * puntos; //vector de puntos limites de tramos

signals:
    void closed();

public slots:
    void aceptar();
    void cancelar();

    void introNtramos();
    void introPuntoi();
};

#endif // TRAMOS_H
