#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QTcpSocket>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void on_loginpushbutton_clicked();
    void on_registerpushbutton_clicked();
    void on_connectpushbutton_clicked();

private:
    Ui::login *ui;
    QTcpSocket *socket = new QTcpSocket;
    bool connectstatement = false ;
};

#endif // LOGIN_H
