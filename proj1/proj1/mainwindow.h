#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDialog>
#include <fstream>
#include <ostream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
//#include "myFun.h"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void myupdate();
    void storeimg();
    void outputimg();
    void showimg(QString showedimg);
    void showgif();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
