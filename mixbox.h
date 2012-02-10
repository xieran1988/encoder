#ifndef MIXBOX_H
#define MIXBOX_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/ChildProxy>
#include <QGst/PropertyProbe>
#include <QGst/Pad>
class MIXBox : public QWidget
{
    Q_OBJECT
public:
    explicit MIXBox(QWidget *parent = 0);
    QPropertyAnimation *animation;
    QRect boxRect;
    int index;
    QGst::PipelinePtr *pip;
    void AnimationStart(QRect rt,int Duration=1000);
    void AnimationSwap(MIXBox *obj,int Duration=1000);
signals:
    
public slots:
    void onValueChanged(QVariant value);
};

#endif // MIXBOX_H
