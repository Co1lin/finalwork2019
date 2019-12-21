#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myFun.h"
#include <QCoreApplication>
using namespace std;

map<string, myOpt> tokens;
vector<myVar> vars;
vector<codeBlock> funs;
bool in_block = 0;
ifstream infile;
ofstream errorLog;
Turtle turtle = { 0, 0, 0, 1 };

QString current_path;
QString input_file;
QString error_log;
QString output_file("");
QImage* myimage = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 实例化QPixmap
    QPixmap pimage;
    // 给QPixamap加载图片
    if (myimage != NULL)
        pimage.convertFromImage(*myimage);
    //pimage.load(output_file);
    // 将图片展示到QLabel上
    ui->label->setPixmap(myimage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int mainProcess(QPainter &painter, QImage &image);

void MainWindow::paintEvent(QPaintEvent *)
{
    //Initial
    current_path = QCoreApplication::applicationDirPath();
    if (current_path[1] != ':') //for Mac
        current_path += "/../../..";
    input_file = current_path + "/input.logo";
    error_log = current_path + "/errorLog.txt";
    output_file = current_path + "/output.bmp";
    //infile.open("/Users/colin/github/finalwork2019/proj0/test0.logo");
    infile.open(input_file.toStdString());
    errorLog.open(error_log.toStdString());
    int width = 1920, height = 1080;
    int r = 0, g = 0, b = 0;
    //int xpos = 0, ypos = 0;
    readHead(width, height, r, g, b, turtle.x, turtle.y);
    QImage image(width, height, QImage::Format_ARGB32);
    myimage = &image;
    image.fill({r, g, b});
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setColor(QColor(0, 0, 0));
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    ini_list();
    //Draw Picture!
    mainProcess(painter, image);

    //Save
    //QString output_file = current_path + "/output.bmp";
    image.save(output_file);
    this->close();
}

int mainProcess(QPainter &painter, QImage &image)
{
    vector<myCmd>::iterator fake;
    while (1)
    {
        myCmd cmd;
        int ret = readLine(cmd);
        if (ret > 0)
        {
            myExe(cmd, painter, fake);
            //image.save(output_file);
        }
        else if (ret == -1) //read invalid token
            errorOccurred("In \"mainProcess\": Read invalid operation: ");
        else
            break;
    }
    return 0;
}
