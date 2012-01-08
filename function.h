#ifndef FUNCTION_H
#define FUNCTION_H

#include <QWidget>

#define SIMPLE_MODE 0
//#define X_MODE 1
#define Y_MODE 2
//#define TOTAL_MODE 3
//atencion: aun no se han usado los modos X_MODE y TOTAL_MODE

class Function : public QWidget
{
    Q_OBJECT

public:
    explicit Function(QWidget *parent = 0, QVector<double> func = QVector<double>(),
                      double x_min = 0.0, double x_max = 1.0, double y_min = 0.0, double y_max = 1.0,
                      int mode = SIMPLE_MODE);
    ~Function();

    double getX();
    double getY();

private:
    QVector<double> fun; //vector que representa la funcion
    double xmin, xmax;
    double ymin, ymax;
    int modo; //modo de visualizar: que partes de los ejes x e y se muestran

    double xMouse, yMouse; //valor (x, y) donde esta el raton


protected:
    void paintEvent(QPaintEvent *event);
    //void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);


signals:
    void moused();
};

#endif // FUNCTION_H
