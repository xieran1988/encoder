#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include <mydata.h>
#include "gstmux.h"
#include "gstmix.h"
#include <QList>
#include "dlggroup.h"

class Group : public QObject
{
    Q_OBJECT
public:
    explicit Group(QObject *parent = 0);
    ~Group();
    _group *data;
    bool init();
    bool startMix(int index, int type);
    bool startMux(int index, int type);
    bool stopMix(int index);
    bool stopMux(int index);
    bool pauseMix(int index);
    bool pauseMux(int index);
    int getMixState(int index);
    int getMuxState(int index);
    QString& getAllState();
    DLGGroup *dlg;
private:
    QList<GSTMUX*> mux;
    QList<GSTMIX*> mix;


signals:

    
public slots:
    
};

#endif // GROUP_H
