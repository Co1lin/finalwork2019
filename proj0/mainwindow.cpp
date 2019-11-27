#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myFun.h"
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

int mainProcess(ifstream &infile, QPainter &painter);

void MainWindow::paintEvent(QPaintEvent *)
{
    //Initial
    ifstream infile;
    infile.open("/Users/colin/github/finalwork2019/proj0/test0.logo");
    int width = 1920, height = 1080;
    int r = 0, g = 0, b = 0;
    int xpos = 0, ypos = 0;
    readHead(infile, width, height, r, g, b, xpos, ypos);
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill({r, g, b});
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    ini_list();
    //Draw Picture!
    mainProcess(infile, painter);

    //Save
    image.save("/Users/colin/github/finalwork2019/proj0/test0.bmp");
}

int mainProcess(ifstream &infile, QPainter &painter)
{
    while (1)
    {
        myCmd cmd;
        int ret = readLine(infile, cmd);
        if (!ret)
        {
            myExe(cmd, painter);
        }
        else
            break;
    }
    return 0;
}


