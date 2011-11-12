#ifndef AYUDA_H
#define AYUDA_H

#include <QWidget>

namespace Ui {
    class Ayuda;
}

class Ayuda : public QWidget
{
    Q_OBJECT

public:
    explicit Ayuda(QWidget *parent = 0);
    ~Ayuda();


private slots:
    void on_findButton_clicked();

private:
    Ui::Ayuda *ui;
    void loadTextFile();
};

#endif // AYUDA_H

