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

bool Group::startMix(int index)
{
    return mix.at(index)->start();
}

bool Group::startMux(int index)
{
    return mux.at(index)->start();
}
