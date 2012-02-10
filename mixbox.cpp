#include "mixbox.h"

MIXBox::MIXBox(QWidget *parent) :
    QWidget(parent)
{
    animation=new QPropertyAnimation(this,"geometry");
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->setLoopCount(1);
    this->connect(animation,SIGNAL(valueChanged(QVariant)),this,SLOT(onValueChanged(QVariant)));
}

void MIXBox::AnimationStart(QRect rt,int Duration)
{
    if(animation->state()==QAbstractAnimation::Running)
    {
        animation->stop();
    }
    //Animation->setStartValue(this->geometry());
    animation->setEndValue(rt);
    animation->setDuration(Duration);
    animation->start();

}


void MIXBox::AnimationSwap(MIXBox *obj,int Duration)
{
    if(animation->state()==QAbstractAnimation::Running ||
       obj->animation->state()==QAbstractAnimation::Running)
    {
        QRect rt1=animation->endValue().toRect();
        QRect rt2=obj->animation->endValue().toRect();
        animation->stop();
        obj->animation->stop();
        AnimationStart(rt2,Duration);
        obj->AnimationStart(rt1,Duration);
    }
    else
    {
        AnimationStart(obj->geometry(),Duration);
        obj->AnimationStart(geometry(),Duration);
    }
}

void MIXBox::onValueChanged(QVariant value)
{
    QRect rt=value.toRect();
    QString mixName;
    mixName=QString("sink_%1").arg(index+1);
    (*pip)->getElementByName("mix")->getStaticPad(mixName.toAscii().data())->setProperty("xpos", rt.x());
    (*pip)->getElementByName("mix")->getStaticPad(mixName.toAscii().data())->setProperty("ypos", rt.y());

    QString scaleName;
    QString scaleValue;
    scaleName=QString("scale%1").arg(index+1);
    scaleValue=QString("video/x-raw-yuv, width=%1, height=%2")
                        .arg(animation->endValue().toRect().width())
                        .arg(animation->endValue().toRect().height());
    //qDebug("%s  %s",scaleName.toAscii().data() ,scaleValue.toAscii().data());
    (*pip)->getElementByName(scaleName.toAscii().data())->setProperty("caps", QGst::Caps::fromString(scaleValue));
}
