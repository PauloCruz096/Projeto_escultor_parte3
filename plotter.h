#ifndef PLOTTER_H
#define PLOTTER_H

#include "escultor.h"
#include <QWidget>
#include <vector>
#include <QBasicTimer>

class Plotter : public QWidget
{
    Q_OBJECT
private:
    int nline, ncolumn;
    int borderv;
    int borderh;
    int dimCell;
    int sizetela, hightela;
    int sizecell, hightcell;
    bool pressed;
    vector<vector<Voxel>>v;
    QBasicTimer timer;

public:
    explicit Plotter(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void setTamanho(int nlinhas, int ncolunas);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void loadMatrix(vector<vector<Voxel>>m);

signals:
    void updatePos(int, int);

};

#endif // PLOTTER_H
