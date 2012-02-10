#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vbox=ui->verticalLayout;
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::init()
{
    group.clear();
    Group *tgroup;
    for(int i=0;i<myData.group.count();i++)
    {
        tgroup=new Group();
        tgroup->data=myData.group.at(i);
        tgroup->init();
        group.push_back(tgroup);
    }

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for(int i=0;i<group.count();i++)
    {
        delete group.at(i);
    }
    group.clear();
}
