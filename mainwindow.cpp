#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "escultor.h"
#include "sculptorsize.h"
#include "plotter.h"
#include <iostream>
#include <QPalette>
#include <QColorDialog>
#include <QColor>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dimX = dimY = dimZ = 50;
    r = g = b = 128;
    currentObject = VOXEL;
    e = new Sculptor (dimX, dimY, dimZ);
    ui->sliderX->setMaximum(dimX-1);
    ui->sliderY->setMaximum(dimY-1);
    ui->sliderZ->setMaximum(dimZ-1);
    ui->actionXY->trigger();
    updateColor();
    ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(),XY));
    currentPlane=XY;
    actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actioninsert_voxel);
    actionGroup->addAction(ui->actionremove_voxel);
    actionGroup->addAction(ui->actioninsert_box);
    actionGroup->addAction(ui->actionremove_box);
    actionGroup->addAction(ui->actioninsert_sphere);
    actionGroup->addAction(ui->actionremove_sphere);
    actionGroup->addAction(ui->actioninsert_ellipsoid);
    actionGroup->addAction(ui->actionremove_ellipsoid);
    ui->actioninsert_voxel->setChecked(true);
    actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionXY);
    actionGroup->addAction(ui->actionXZ);
    actionGroup->addAction(ui->actionYZ);
    ui->actionXY->setChecked(true);
    showMaximized();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateColor(){
    QString color, textcolor;
    textcolor = "QPushButton {background - color: rgb(" + QString().setNum(255-r) + "," + QString().setNum(255-g) + "," + QString().setNum(255-b) + ");}";
    color =  "QPushButton {background - color: rgb(" + QString().setNum(r) + "," + QString().setNum(g) + "," + QString().setNum(b) + ");}";
    ui->pushButtonColor->setStyleSheet(color);
    e->setColor(r, g, b, 255);
}

void MainWindow::setR(int _r){
    r = _r;
    updateColor();
}

void MainWindow::setG(int _g){
    g = _g;
    updateColor();
}

void MainWindow::setB(int _b){
    b = _b;
    updateColor();
}

void MainWindow::pickColor(){
    QColorDialog d;
    d.exec();
    r = d.selectedColor().red();
    g = d.selectedColor().green();
    b = d.selectedColor().blue();
    ui->sliderR->setValue(r);
    ui->sliderG->setValue(g);
    ui->sliderB->setValue(b);
    updateColor();
}

void MainWindow::drawShape(int horizClick, int vertClick){
    switch (currentObject) {
    case(VOXEL):
        switch (currentPlane) {
        case XY:
            e->putVoxel(horizClick, vertClick, ui->sliderZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->putVoxel(horizClick, ui->sliderY->value(), vertClick);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->putVoxel(ui->sliderX->value(), horizClick, vertClick);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(NOVOXEL):
        switch (currentPlane) {
        case XY:
            e->cutVoxel(horizClick, vertClick, ui->sliderZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->cutVoxel(horizClick, ui->sliderY->value(), vertClick);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->cutVoxel(ui->sliderX->value(), horizClick, vertClick);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(BOX):
        switch (currentPlane) {
        case XY:
            e->putBox(horizClick-ui->sliderDimX->value()/2, horizClick+ui->sliderDimX->value()/2,
                      vertClick-ui->sliderDimY->value()/2, vertClick+ui->sliderDimY->value()/2,
                      ui->sliderZ->value()-ui->sliderDimZ->value()/2, ui->sliderZ->value()+ui->sliderDimZ->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->putBox(horizClick-ui->sliderDimX->value()/2, horizClick+ui->sliderDimX->value()/2,
                      ui->sliderY->value()-ui->sliderDimY->value()/2, ui->sliderY->value()+ui->sliderDimY->value()/2,
                      vertClick-ui->sliderDimZ->value()/2, vertClick+ui->sliderDimZ->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->putBox(ui->sliderX->value()-ui->sliderDimX->value()/2, ui->sliderX->value()+ui->sliderDimX->value()/2,
                      vertClick-ui->sliderDimZ->value()/2, vertClick+ui->sliderDimZ->value()/2,
                      horizClick-ui->sliderDimY->value()/2, horizClick+ui->sliderDimY->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(NOBOX):
        switch (currentPlane) {
        case XY:
            e->cutBox(horizClick-ui->sliderDimX->value()/2, horizClick+ui->sliderDimX->value()/2,
                      vertClick-ui->sliderDimY->value()/2, vertClick+ui->sliderDimY->value()/2,
                      ui->sliderZ->value()-ui->sliderDimZ->value()/2, ui->sliderZ->value()+ui->sliderDimZ->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->cutBox(horizClick-ui->sliderDimX->value()/2, horizClick+ui->sliderDimX->value()/2,
                      ui->sliderY->value()-ui->sliderDimY->value()/2, ui->sliderY->value()+ui->sliderDimY->value()/2,
                      vertClick-ui->sliderDimZ->value()/2, vertClick+ui->sliderDimZ->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->cutBox(ui->sliderX->value()-ui->sliderDimX->value()/2, ui->sliderX->value()+ui->sliderDimX->value()/2,
                      vertClick-ui->sliderDimZ->value()/2, vertClick+ui->sliderDimZ->value()/2,
                      horizClick-ui->sliderDimY->value()/2, horizClick+ui->sliderDimY->value()/2);
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(SPHERE):
        switch (currentPlane) {
        case XY:
            e->putSphere(horizClick, vertClick, ui->sliderZ->value(), ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->putSphere(horizClick, ui->sliderY->value(), vertClick,  ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->putSphere(ui->sliderX->value(), horizClick, vertClick, ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(NOSPHERE):
        switch (currentPlane) {
        case XY:
            e->cutSphere(horizClick, vertClick, ui->sliderZ->value(), ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->cutSphere(horizClick, ui->sliderY->value(), vertClick,  ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->cutSphere(ui->sliderX->value(), horizClick, vertClick, ui->sliderRaio->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(ELLIPSOID):
        switch (currentPlane) {
        case XY:
            e->putEllipsoid(horizClick, vertClick, ui->sliderZ->value(),
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->putEllipsoid(horizClick, ui->sliderY->value(), vertClick,
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->putEllipsoid(ui->sliderX->value(), horizClick, vertClick,
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    case(NOELLIPSOID):
        switch (currentPlane) {
        case XY:
            e->cutEllipsoid(horizClick, vertClick, ui->sliderZ->value(),
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
            break;
        case XZ:
            e->cutEllipsoid(horizClick, ui->sliderY->value(), vertClick,
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
            break;
        case YZ:
            e->cutEllipsoid(ui->sliderX->value(), horizClick, vertClick,
                            ui->sliderRaioX->value(), ui->sliderRaioY->value(), ui->sliderRaioZ->value());
            ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
            break;
        }
        break;
    }
}

void MainWindow::updateCoords(int horizClick, int vertClick){
    switch (currentPlane) {
    case XY:
        ui->labelX->setNum(horizClick);
        ui->labelY->setNum(vertClick);
        break;
    case XZ:
        ui->labelX->setNum(horizClick);
        ui->labelZ->setNum(vertClick);
        break;
    case YZ:
        ui->labelY->setNum(horizClick);
        ui->labelZ->setNum(vertClick);
        break;
    }
}

void MainWindow::save_triggered(){
    e->writeOFF("Escultura.off");
}

void MainWindow::new_triggered(){
    SculptorSize *dialog;
    dialog = new SculptorSize();
    dialog->exec();
    if(dialog->result() == QDialog::Accepted){
        dimX = dialog->getDimX();
        dimY = dialog->getDimY();
        dimZ = dialog->getDimZ();
        if(e != NULL) delete e;
        e = new Sculptor(dimX, dimY, dimZ);
        ui->actionXY->trigger();
        ui->sliderX->setMaximum(dimX-1);
        ui->sliderY->setMaximum(dimY-1);
        ui->sliderZ->setMaximum(dimZ-1);
    }
}

void MainWindow::insert_voxel_triggered(){
    currentObject = VOXEL;
}

void MainWindow::remove_voxel_triggered(){
    currentObject = NOVOXEL;
}

void MainWindow::insert_box_triggered(){
    currentObject = BOX;
}

void MainWindow::remove_box_triggered(){
    currentObject = NOBOX;
}

void MainWindow::insert_sphere_triggered(){
    currentObject = SPHERE;
}

void MainWindow::remove_sphere_triggered(){
    currentObject = NOSPHERE;
}

void MainWindow::insert_ellipsoid_triggered(){
    currentObject = ELLIPSOID;
}

void MainWindow::remove_ellipsoid_triggered(){
    currentObject = NOELLIPSOID;
}

void MainWindow::surfaceXY(){
    ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderZ->value(), XY));
    currentPlane = XY;
}

void MainWindow::surfaceXZ(){
    ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderY->value(), XZ));
    currentPlane = XZ;
}

void MainWindow::surfaceYZ(){
    ui->widgetDraw->loadMatrix(e->getPlano(ui->sliderX->value(), YZ));
    currentPlane = YZ;
}

void MainWindow::sliderX_value(int value){
    if(currentPlane == YZ) ui->widgetDraw->loadMatrix(e->getPlano(value, YZ));
}

void MainWindow::sliderY_value(int value){
    if(currentPlane == XZ) ui->widgetDraw->loadMatrix(e->getPlano(value, XZ));
}

void MainWindow::sliderZ_value(int value){
    if(currentPlane == XY) ui->widgetDraw->loadMatrix(e->getPlano(value, XY));
}

void MainWindow::on_actionMeshlab_triggered(){
    e->writeOFF("Escultura.off");
    QStringList arguments;
    QString program("C:/Program Files/VCG/MeshLab/meshlab");
    arguments << "Escultura.off";
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
}
