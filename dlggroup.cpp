#include "dlggroup.h"
#include "ui_dlggroup.h"
DLGGroup::DLGGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLGGroup)
{
    ui->setupUi(this);
    vbox=ui->verticalLayout;
}

DLGGroup::~DLGGroup()
{
    delete ui;
}
