#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myFun.h"
#include <QCoreApplication>
#include <thread>
#include <sstream>
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
MainWindow* wtf;
vector<QImage> imgs;

int iii = 0;
int iiii = 0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->label->setNum(iiii);
    ui->label->resize(this->size());
}

void MainWindow::myupdate()
{
    QPixmap pm;
    pm.convertFromImage(*myimage);
    //ui->label->setPixmap(pm);
    //ui->label->update();
    //ui->label->setNum(++iiii);
    iiii++;
    if (iiii % 100 == 0)
    {
        iii++;
        stringstream s_index;
        s_index << iii;
        string s_iiii;
        s_index >> s_iiii;
        QString mysave = current_path + "/traces/" + QString::fromStdString(s_iiii) + ".jpg";
        //QString mysave = current_path + "/" + QString::fromStdString(s_iiii) + ".bmp";
        //cout << mysave.toStdString() << endl;
        myimage->save(mysave);
        iiii = 0;
    }
}

void MainWindow::storeimg()
{
    iiii++;
    if (iiii % 250 == 0)
    {
        imgs.push_back(*myimage);
        iiii = 0;
    }
}

void MainWindow::outputimg()
{
    int index = 0;
    auto iter = imgs.begin();
    for (; iter != imgs.end(); iter++)
    {
        index++;
        stringstream ss_index;
        ss_index << index;
        string s_index;
        ss_index >> s_index;
        QString mysave = current_path + "/traces/" + QString::fromStdString(s_index) + ".jpg";
        iter->save(mysave);
    }
}

void MainWindow::showimg(QString showedimg)
{
    ui->label->resize(this->size());
    QPixmap pm;
    pm.load(showedimg);
    QPainter showing(this);
    showing.drawPixmap(0, 0, ui->label->width(), ui->label->height(), pm);
    this->update();
}

void MainWindow::showgif()
{
    QString tobeshow = current_path + "/traces/";
    for (int i = 1; i <= imgs.size(); i++)
    {
        stringstream ss_index;
        ss_index << i;
        string s_index;
        ss_index >> s_index;
        showimg(tobeshow + QString::fromStdString(s_index) + ".jpg");
        system("pause");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int mainProcess(QPainter &painter, QImage &image);

void MainWindow::paintEvent(QPaintEvent *)
{
    while (!iii)
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
        imgs.push_back(image);
        //Save
        //QString output_file = current_path + "/output.bmp";
        image.save(output_file);
        outputimg();
        iii = 1;
    }
    showimg(output_file);
    //showgif();
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
