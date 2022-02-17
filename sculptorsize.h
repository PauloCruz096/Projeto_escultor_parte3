#ifndef SCULPTORSIZE_H
#define SCULPTORSIZE_H

#include <QDialog>
#include <QString>

namespace Ui {
class SculptorSize;
}

class SculptorSize : public QDialog
{
    Q_OBJECT

public:
    explicit SculptorSize(QWidget *parent = nullptr);
    ~SculptorSize();
    int getDimX();
    int getDimY();
    int getDimZ();

private:
    Ui::SculptorSize *ui;

};

#endif // SCULPTORSIZE_H
