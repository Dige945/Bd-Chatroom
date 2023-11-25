#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QString i,QWidget *parent = nullptr);
    ~Widget();

    void on_SendpushButton_clicked();
    void on_ConnectpushButton_clicked();
    void readMessage();
    void disConnectslot();
    void setuserid (QString &id);
    void updateuserlist(const QString&);
    void pause(int) ;

    //私聊功能的实现
    void startprivatechat(QTableWidgetItem*);
    QString userid="username";//用户名
private:
    Ui::Widget *ui;
    QTcpSocket *socket;
    bool connectstatement=false;

};
#endif // WIDGET_H
