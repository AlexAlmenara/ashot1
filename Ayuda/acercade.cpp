#include "acercade.h"
#include "ui_acercade.h"

Acercade::Acercade(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acercade)
{
    ui->setupUi(this);
}

Acercade::~Acercade()
{
    delete ui;
}
