#ifndef DLGMIX_H
#define DLGMIX_H

#include <QWidget>
#include <QFormLayout>
namespace Ui {
class DLGMIX;
}

class DLGMIX : public QWidget
{
    Q_OBJECT
    
public:
    explicit DLGMIX(QWidget *parent = 0);
    ~DLGMIX();
    QFormLayout *fbox;

signals:
    void start();
    void stop();
    void showSignal();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_show_clicked();

private:
    Ui::DLGMIX *ui;
};

#endif // DLGMIX_H
