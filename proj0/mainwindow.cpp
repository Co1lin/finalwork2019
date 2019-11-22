#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QFile>
#include <QDir>
#include <cmath>


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

void MainWindow::paintEvent(QPaintEvent *)
{
    //Initial
    std::ifstream infile;
    infile.open("/Users/colin/github/finalwork2019/proj0/test0.logo");
    int width = 0, height = 0;
    int r, g, b;
    int xpos = 0, ypos = 0;
    readHead(infile, width, height, r, g, b, xpos, ypos);
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill({r, g, b});
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //Draw Picture!

    //Save
    image.save("/Users/colin/github/finalwork2019/proj0/test0.bmp");
}

//by Colin
//There are too many arguments! It seems ugly, but it CAN run whatever.
//So, let it be!
int MainWindow::readHead(std::ifstream &infile, int &width, int &height, int &r, int &g, int &b, int &xpos, int &ypos)
{
    std::string in;
    std::getline(infile, in);
    //@SIZE 1920 1080
    if (in.substr(0, 5) == "@SIZE") //substr(a, b) -> [a, b)
    {
        //unsigned long first = in.find_first_of(' ');
        unsigned long first = 5;
        unsigned long second = in.find_last_of(' ');
        int w = s2num(in.substr(first + 1, second - first - 1));
        int h = s2num(in.substr(second + 1));
        if (320 <= w && w <= 1920 && 240 <= h && h <= 1080)
        {
            width = w;
            height = h;
        }
        else
            errorOccurred();
    }
    else
        errorOccurred();

    //@BACKGROUND 255 255 255
    std::getline(infile, in);
    if (in.substr(0, 11) == "@BACKGROUND")
    {
        int i = 0;
        //R
        std::string tmp;
        for (i = 12; i <= in.length(); i++)
        {
            if ('0' <= in[i] && in[i] <= '9')
                tmp += in[i];
            else if (in[i] == ' ')
            {
                r = s2num(tmp);
                break;
            }
        }
        //G
        tmp = "";
        for (i++; i <= in.length(); i++)
        {
            if ('0' <= in[i] && in[i] <= '9')
                tmp += in[i];
            else if (in[i] == ' ')
            {
                g = s2num(tmp);
                break;
            }
        }
        //B
        tmp = "";
        for (i++; i <= in.length(); i++)
        {
            if ('0' <= in[i] && in[i] <= '9')
                tmp += in[i];
            else if (in[i] == '\0')
            {
                b = s2num(tmp);
                break;
            }
        }
        //Check
        if (0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255)
            ;
        else
            errorOccurred();
    }
    else
        errorOccurred();

    //@POSITION 959 10
    std::getline(infile, in);
    if (in.substr(0, 9) == "@POSITION")
    {
        unsigned long first = 9;
        unsigned long second = in.find_last_of(' ');
        int x = s2num(in.substr(first + 1, second - first - 1));
        int y = s2num(in.substr(second + 1));
        if (0 <= x && x <= width - 1 && 0 <= y && y <= height - 1)
        {
            xpos = x;
            ypos = y;
        }
        else
            errorOccurred();
    }
    else
        errorOccurred();

    return 0;
}

void MainWindow::errorOccurred()
{
    std::cout << "Error occurred!\n";
}
