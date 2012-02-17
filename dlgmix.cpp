#include "dlgmix.h"
#include "ui_dlgmix.h"

DLGMIX::DLGMIX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLGMIX)
{
    ui->setupUi(this);
    fbox=ui->formLayout;
}

DLGMIX::~DLGMIX()
{
    delete ui;
}

void DLGMIX::on_pushButton_start_clicked()
{
    emit start();
}

void DLGMIX::on_pushButton_stop_clicked()
{
    emit stop();
}

void DLGMIX::on_pushButton_show_clicked()
{
    emit showSignal();
}

void DLGMIX::on_pushButton_pause_clicked()
{
    emit pause();
}
