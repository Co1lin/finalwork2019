#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
