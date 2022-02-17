#include "sculptorsize.h"
#include "ui_sculptorsize.h"

SculptorSize::SculptorSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SculptorSize)
{
    ui->setupUi(this);
}

SculptorSize::~SculptorSize()
{
    delete ui;
}


int SculptorSize::getDimX()
{
    return ui->spinBoxX->value();
}

int SculptorSize::getDimY()
{
    return ui->spinBoxY->value();
}

int SculptorSize::getDimZ()
{
    return ui->spinBoxZ->value();
}
