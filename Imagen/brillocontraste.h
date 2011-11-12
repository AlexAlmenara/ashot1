#ifndef BRILLOCONTRASTE_H
#define BRILLOCONTRASTE_H

#include <QWidget>

namespace Ui {
    class BrilloContraste;
}

class BrilloContraste : public QWidget
{
    Q_OBJECT

public:
    explicit BrilloContraste(QWidget *parent = 0);
    ~BrilloContraste();


public slots:
    void AplicarCambios();

private:
    Ui::BrilloContraste *ui;
    //int Brillo;
    //int Contraste;
};

#endif // BRILLOCONTRASTE_H
