#include "confirmkickyourass.h"
#include "ui_confirmkickyourass.h"
#include "mainwindow.h"

confirmkickyourass::confirmkickyourass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::confirmkickyourass)
{
    ui->setupUi(this);
}

confirmkickyourass::~confirmkickyourass()
{
    delete ui;
}
