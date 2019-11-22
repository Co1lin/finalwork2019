#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <fstream>
#include <iostream>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int s2num(std::string s)
    {
        int ten = 1;
        int l = s.length();
        int num = 0;
        for (int i = l - 1; i >= 0; i--)
        {
            num += (s[i] - '0') * ten;
            ten *= 10;
        }
        return num;
    }
    void errorOccurred();
    void paintEvent(QPaintEvent *);
    int readHead(std::ifstream &infile, int &width, int &height, int &r, int &g, int &b, int &xpos, int &ypos);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
