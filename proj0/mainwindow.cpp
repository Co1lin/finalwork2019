#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myFun.h"
#include <QCoreApplication>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

map<string, myOpt> tokens;
vector<myVal> vals;
vector<codeBlock> funs;
ifstream infile;
Turtle turtle = { 0, 0, 1 };

int mainProcess(ifstream &infile, QPainter &painter);

void MainWindow::paintEvent(QPaintEvent *)
{
    //Initial
    //ifstream infile;
    //"/Users/colin/github/finalwork2019/build-proj0-Desktop_Qt_5_13_1_clang_64bit-Debug/proj0.app/Contents"
    //QString current_path = QCoreApplication::applicationDirPath();
    QString current_path = "/Users/colin/github/finalwork2019/proj0";
    QString input_file = current_path + "/test0.logo";
    //infile.open("/Users/colin/github/finalwork2019/proj0/test0.logo");
    infile.open(input_file.toStdString());
    int width = 1920, height = 1080;
    int r = 0, g = 0, b = 0;
    //int xpos = 0, ypos = 0;
    readHead(width, height, r, g, b, turtle.x, turtle.y);
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill({r, g, b});
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    ini_list();
    //Draw Picture!
    mainProcess(infile, painter);

    //Save
    QString output_file = current_path + "/test0.bmp";
    image.save(output_file);
}

int mainProcess(ifstream &infile, QPainter &painter)
{
    while (1)
    {
        myCmd cmd;
        int ret = readLine(cmd);
        if (ret > 0)
        {
            myExe(cmd, painter);
        }
        else if (ret == -1) //read invalid token
            errorOccurred();
        else
            break;
    }
    return 0;
}


