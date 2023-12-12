#include "login.h"
#include "ui_login.h"
#include "widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
   // connect(ui->connectpushbutton,&QPushButton::clicked,this,&login::on_connectpushbutton_clicked);
    connect(ui->loginpushbutton,&QPushButton::clicked,this,&login::on_loginpushbutton_clicked);
    connect(ui->registerpushbutton,&QPushButton::clicked,this,&login::on_registerpushbutton_clicked);
}
/*void login::on_connectpushbutton_clicked(){
        socket->connectToHost("127.0.0.1",8000);
        if(socket->waitForConnected()){
            QMessageBox::information(this,"Information","连接成功！");
            connectstatement = true ;
            socket->close();
        }
        else {
            QMessageBox::information(this,"Warning","连接失败!");
            socket->close();
        }
}*/

//下面是数据库部分，当时一直连接不上，然后用了ODBC（一个应用）中转后连上了，要编译的时候这部分全部注释掉就行
void login::on_loginpushbutton_clicked(){
    socket->connectToHost("127.0.0.1",8001);//自行设置端口和地址
    if(socket->waitForConnected()){
        QMessageBox::information(this,"Information","连接服务器成功！");
        connectstatement = true ;
        socket->close();
    }
    else {
        QMessageBox::information(this,"Warning","连接服务器失败!");
        socket->close();
    }

    if(connectstatement){
        //登录按钮
        QString id;
        QString password;
        id= ui->username->toPlainText();
        password=ui->password->toPlainText();
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("chatroom.db");
        //原来的mysql数据库的数据代码
 /*           db.setHostName("127.0.0.1");
            db.setPort(3306);
            db.setDatabaseName("chatroom");
            db.setUserName("root");
            db.setPassword("******");  */
        if (db.open()) {
            qDebug() << "连接MySQL数据库成功";
            QMessageBox::information(this,"提示","连接MySQL数据库成功");
            QSqlQuery query(db);
            if (!query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT)")) {
                qDebug() << "无法创建 users 表: " << query.lastError().text();
            }
        } else {
            QMessageBox::information(this,"提示","连接MySQL数据库失败");
            qDebug() << "连接MySQL数据库失败";
        }
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE username = :id AND password = :password");
        query.bindValue(":id", id);
        query.bindValue(":password", password);
        if (query.exec() && query.next())
        {
            //登陆成功后，把Id发送给服务器

            Widget *w = new Widget(id);
            w->show();
            qDebug() << "登录成功";
            this->close();

        }
        else
        {
        QMessageBox::information(this, "错误", "用户名或密码不对或未注册", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::information(this,"警告","未连接服务器，请先连接服务器");
    }
}

void login::on_registerpushbutton_clicked(){
    if(connectstatement){
        //注册按钮，这个还没调试完
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("chatroom.db");
        //原有的mysql数据库的代码
   /*     db.setHostName("127.0.0.1");//127.0.0.1
        db.setPort(3306);
        db.setUserName("root");
        db.setPassword("******");
        db.setDatabaseName("chatroom"); //这个库是存在的  */
        if (db.open()) {
            qDebug() << "连接MySQL数据库成功";

            QSqlQuery query(db);
            if (!query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT, password TEXT)")) {
                qDebug() << "无法创建 users 表: " << query.lastError().text();
            }
        } else {
            qDebug() << "连接MySQL数据库失败";
        }
        QSqlQuery query(db);
        QString id = ui->username->toPlainText();
        QString password = ui->password->toPlainText();
        if(id == ""|| password == ""){
            qDebug() <<"输入为空";
            QMessageBox::information(this, "错误", "输入为空", QMessageBox::Ok);
            return;
        }
        else{
            query.prepare("SELECT username FROM users WHERE username = ':id' ");
            query.bindValue(":id", id);
            if (query.exec() && query.next()) {
                qDebug() << "用户名已存在，请选择其他用户名";
                // 显示注册失败错误消息框
                QMessageBox::information(this, "错误", "用户名已存在，请重新输入", QMessageBox::Ok);
                return ;
            }
            else{
                query.prepare("INSERT INTO users (username, password) VALUES (:id, :password)");
                query.bindValue(":id", id);
                query.bindValue(":password", password);
                if (query.exec()) {
                    qDebug() << "用户注册成功";
                    QMessageBox::information(this, "欢迎", "注册账号成功，去登录", QMessageBox::Ok);
                }
                else {
                    qDebug() << "用户注册失败" << query.lastError().text();
                    QMessageBox::information(this,"警告","注册失败,用户名已存在",QMessageBox::Ok);

                }
            }
        }
        db.close();
    }
    else{
        QMessageBox::information(this,"警告","未连接服务器，请先连接服务器");
    }
}
login::~login()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Closing socket connection in destructor";
        socket->close();
    }
    delete ui;
}
