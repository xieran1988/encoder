#include "dlgmux.h"
#include "ui_dlgmux.h"

DLGMUX::DLGMUX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLGMUX)
{
    ui->setupUi(this);
    fbox=ui->formLayout;
}

DLGMUX::~DLGMUX()
{
    delete ui;
}

void DLGMUX::on_pushButton_start_clicked()
{
    emit start();
}

void DLGMUX::on_pushButton_stop_clicked()
{
    emit stop();
}
