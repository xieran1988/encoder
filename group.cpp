#include "group.h"
#include "mainwindow.h"
Group::Group(QObject *parent) :
    QObject(parent)
{
    dlg=new DLGGroup();
    myWindow->vbox->addWidget(dlg);
    dlg->show();
}

Group::~Group()
{
    for(int i=0;i<data->mux.count();i++)
    {
        delete mux.at(i);
    }
    mux.clear();
}

bool Group::init()
{

    //init mux
    mux.clear();

    GSTMIX *tmix;
    for(int i=0;i<data->mix.count();i++)
    {
        tmix=new GSTMIX(data->mix.at(i), this);
        ///tmix->showData();
        mix.push_back(tmix);
    }

    GSTMUX *tmux;
    for(int i=0;i<data->mux.count();i++)
    {
        tmux=new GSTMUX(data->mux.at(i), this);
        tmux->showData();
        mux.push_back(tmux);
    }


    return true;
}

bool Group::startMix(int index, int type)
{
    return mix.at(index)->start(type);
}

bool Group::startMux(int index, int type)
{
    return mux.at(index)->start(type);
}

bool Group::stopMix(int index)
{
    return mix.at(index)->stop();
}

bool Group::stopMux(int index)
{
    return mux.at(index)->stop();
}
bool Group::pauseMix(int index)
{
    return mix.at(index)->pause();
}

bool Group::pauseMux(int index)
{
    return mux.at(index)->pause();
}

int Group::getMixState(int index)
{
    return mix.at(index)->getState();
}
int Group::getMuxState(int index)
{
    return mux.at(index)->getState();
}

QString& Group::getAllState()
{
    QString str;
    for(int i=0;i<mix.count();i++)
    {
        str+=QString("%1,").arg(mix.at(i)->getState());
    }
    for(int i=0;i<mux.count();i++)
    {
        str+=QString("%1,").arg(mux.at(i)->getState());
    }

    str=str.left(str.length()-1);
    qDebug("%s",str.toAscii().data());
    return str;
}
