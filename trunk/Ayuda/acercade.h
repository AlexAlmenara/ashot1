#ifndef ACERCADE_H
#define ACERCADE_H

#include <QWidget>

namespace Ui {
    class Acercade;
}

class Acercade : public QWidget
{
    Q_OBJECT

public:
    explicit Acercade(QWidget *parent = 0);
    ~Acercade();

private:
    Ui::Acercade *ui;
};

#endif // ACERCADE_H
