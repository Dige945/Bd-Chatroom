#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QAbstractSocket>
#include <QDateTime>
#include <QNetworkInterface>
#include <QIcon>
#include <QMessageBox>
#include <QTimer>
#include <QTableWidget>
#include <privatechatwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void enumAllIp();
    void pause(int);
public slots:
    void newConnectSlot();
    void disConnectSlot();
    void readMessage();

    void on_listeningbtn_clicked();
    void on_sendbtn_clicked();

    //在线人数的控制
    void on_usernumbermodify();
    //客户端的进出更新到表中
    void userEnter(QString);
    void userout(QString);
    //更新客户端的列表
    void updateuserlist();
    // 私聊界面所需要的函数
    void startPrivatechat(QTableWidgetItem *item);
    void initPrivatechat(QTcpSocket*,const QString&,MainWindow*);

signals:
    void usernumbermodify();
    void start_usernametable();
    void userlistupdated();


private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *clientsock;
    QList <QTcpSocket*> clientList_sock;
    static int clientnum;

    QMap <QTcpSocket*,QString>socketUserMap;
};
#endif // MAINWINDOW_H
