#ifndef DLGMUX_H
#define DLGMUX_H

#include <QWidget>
#include <QFormLayout>
namespace Ui {
class DLGMUX;
}

class DLGMUX : public QWidget
{
    Q_OBJECT
    
public:
    explicit DLGMUX(QWidget *parent = 0);
    ~DLGMUX();
    QFormLayout *fbox;
signals:
    void start();
    void stop();
    
private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::DLGMUX *ui;
};

#endif // DLGMUX_H
