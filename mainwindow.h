#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "escultor.h"
#include "sculptorsize.h"
#include "plotter.h"

enum {
  VOXEL, NOVOXEL, BOX, NOBOX, SPHERE, NOSPHERE, ELLIPSOID, NOELLIPSOID
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Sculptor *e;
    int dimX,dimY,dimZ;
    int r,g,b,a;
    int currentObject;
    int currentPlane;
    void updateColor();
    QActionGroup *actionGroup;

public slots:
    void setR(int _r);
    void setG(int _g);
    void setB(int _b);
    void pickColor();
    void drawShape(int horizClick, int vertClick);
    void updateCoords(int horizClick, int vertClick);

private slots:
    void save_triggered();
    void new_triggered();
    void insert_voxel_triggered();
    void remove_voxel_triggered();
    void insert_box_triggered();
    void remove_box_triggered();
    void insert_sphere_triggered();
    void remove_sphere_triggered();
    void insert_ellipsoid_triggered();
    void remove_ellipsoid_triggered();
    void surfaceXY();
    void surfaceXZ();
    void surfaceYZ();
    void sliderX_value(int value);
    void sliderY_value(int value);
    void sliderZ_value(int value);
    void on_actionMeshlab_triggered();

};
#endif // MAINWINDOW_H
