#ifndef GSTMUX_H
#define GSTMUX_H

#include <QObject>
#include <mydata.h>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/ChildProxy>
#include <QGst/PropertyProbe>
#include "dlgmux.h"

class GSTMUX : public QObject
{
    Q_OBJECT
public:
    explicit GSTMUX(QObject *parent = 0);
    GSTMUX( _mux *d, QObject *parent = 0);
    ~GSTMUX();
public:
    _mux *data;
    bool start();
    bool stop();
    void showData();
private:
    QGst::PipelinePtr pipeline;
    DLGMUX *dlg;
signals:
    
public slots:
    void onstart();
    void onstop();
    
};

#endif // GSTMUX_H
