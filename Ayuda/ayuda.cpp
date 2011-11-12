#include "ayuda.h"
#include "ui_ayuda.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>

Ayuda::Ayuda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ayuda)
{
    ui->setupUi(this);
    loadTextFile();
}

Ayuda::~Ayuda()
{
    delete ui;
}


void Ayuda::on_findButton_clicked()
{
     QString searchString = ui->lineEdit->text();
     ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
 }



void Ayuda::loadTextFile()
{
     QFile inputFile(":/Ayuda.txt");
     inputFile.open(QIODevice::ReadOnly);

     QTextStream in(&inputFile);
     QString line = in.readAll();
     inputFile.close();

     ui->textEdit->setPlainText(line);
     QTextCursor cursor = ui->textEdit->textCursor();
     cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
 }
