#ifndef PRIVATECHATWIDGET_H
#define PRIVATECHATWIDGET_H

#include <QWidget>
#include <widget.h>
#include <QTcpSocket>
namespace Ui {
class Privatechatwidget;
}

class Privatechatwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Privatechatwidget(QString,QTcpSocket*,QString,QWidget *parent = nullptr);
    ~Privatechatwidget();
    void on_sendbtn_clicked();

private:
    Ui::Privatechatwidget *ui;
    QString chatusername;
    QTcpSocket *socket;
    QString userid;
};

#endif // PRIVATECHATWIDGET_H
