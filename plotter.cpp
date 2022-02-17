#include "plotter.h"
#include <iostream>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QRect>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QGradient>
#include <QSize>


Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    nline = 10;
    ncolumn = 10;
    pressed = false;
    sizetela = rect().width();
    hightela = rect().height();
    sizecell = sizetela/ncolumn;
    hightcell = hightela/nline;
    if(hightcell > sizecell){
        dimCell = sizecell;
    }
    setTamanho(nline, ncolumn);
}

void Plotter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    QColor color;
    QRadialGradient gradient(QPoint(1,1), 20);
    gradient.setColorAt(0, Qt::black);
    gradient.setColorAt(1, Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    pen.setWidth(1);
    sizetela = rect().width();
    hightela = rect().height();
    sizecell = sizetela/ncolumn;
    hightcell = hightela/nline;
    if(hightcell > sizecell){
        dimCell = sizecell;
    }else{
        dimCell = hightcell;
    }
    borderh = (sizetela - ncolumn*dimCell)/2;
    borderv = (hightela - nline*dimCell)/2;
    gradient.setRadius(dimCell);
    gradient.setFocalPoint(-0.15*dimCell, -0.15*dimCell);
    gradient.setCenter(-0.15*dimCell, -0.15*dimCell);
    painter.setPen(pen);
    painter.drawRect(rect());

    for(uint i=0; i<v.size(); i++){
        for(uint j=0; j<v[i].size(); j++){
            if(v[i][j].is0n == true){
                painter.save();
                painter.translate(j*dimCell+borderh+dimCell/2, i*dimCell+borderv+dimCell/2);
                painter.setPen(Qt::NoPen);
                color.setRgba(qRgba(v[i][j].r, v[i][j].g, v[i][j].b, v[i][j].a));
                gradient.setColorAt(0, Qt::white);
                gradient.setColorAt(0.3, color);
                gradient.setColorAt(1, color.darker());
                painter.setBrush(gradient);
                painter.drawEllipse(-dimCell/2+1, -dimCell/2+1, dimCell-1, dimCell-1);
                painter.restore();
            }else{
                brush.setColor(QColor(255,255,255,0));
                painter.setBrush(brush);
                painter.drawRect(j*dimCell+borderh, i*dimCell+borderv, dimCell, dimCell);
            }
        }
    }
}

void Plotter::setTamanho(int nlinhas, int ncolunas){
    nline = nlinhas;
    ncolumn = ncolunas;
    update();
}

void Plotter::mousePressEvent(QMouseEvent *event){
    int posh, posv;
    QRect ret;
    ret = rect();
    pressed = true;
    ret.adjust(borderh, borderv, -borderh, - borderv);
    if(ret.contains(event->pos())){
        posh = (event->pos().x()-borderh)*ncolumn/ret.width();
        posv = (event->pos().y()-borderv)*nline/ret.height();
        emit updatePos(posh, posv);
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event){
    int posh, posv;
    QRect ret;
    ret = rect();
    ret.adjust(borderh, borderv, -borderh, borderv);
    if(ret.contains(event->pos())){
        posh = (event->pos().x()-borderh)*ncolumn/ret.width();
        posv = (event->pos().y()-borderv)*nline/ret.height();
        emit updatePos(posh, posv);
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void Plotter::loadMatrix(vector<vector<Voxel>> m){
    nline = m.size();
    ncolumn = m[0].size();
    v = m;
    repaint();
}
