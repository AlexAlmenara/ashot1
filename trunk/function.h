#ifndef FUNCTION_H
#define FUNCTION_H

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QtGui>
#include <stdio.h>

/*namespace Ui {
    class Function;
}*/

class Function : public QWidget
{
    Q_OBJECT

public:
    explicit Function(QWidget *parent = 0, QVector<double> func = QVector<double>(), double x_min = 0.0, double x_max = 0.0, double y_min = 0.0, double y_max = 0.0);
    ~Function();

    double getX();
    double getY();

private:
    //Ui::Function *ui;

    QVector<double> fun; //vector que representa la funcion
    double xmin, xmax;
    double ymin, ymax;

    //QPoint posMouse;
    double xMouse, yMouse; //valor (x, y) donde esta el raton


protected:
    void paintEvent(QPaintEvent *event);
    //void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);


signals:
    void moused();
};

#endif // FUNCTION_H
