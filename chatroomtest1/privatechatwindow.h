#ifndef PRIVATECHATWINDOW_H
#define PRIVATECHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include "mainwindow.h"

namespace Ui {
class PrivateChatWindow;
}

class MainWindow;
class PrivateChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChatWindow(QTcpSocket*,MainWindow*,QWidget *parent = nullptr);
    ~PrivateChatWindow();

private slots:
    void on_sendbtn_clicked();




private:
    Ui::PrivateChatWindow *ui;
    QTcpSocket *privatesocket,*currentsocket;
    MainWindow *server;

};

#endif // PRIVATECHATWINDOW_H
