#include "connect1.h"
#include "ui_connect.h"
#include "widget.h"
#include "login.h"
connect1::connect1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connect1)
{
    ui->setupUi(this);
    connect(ui->connectButton,&QPushButton::clicked,this,&connect::connectbutton);


}
void connect1::connectbutton(){
    QTcpSocket *socket = new QTcpSocket;
        socket->connectToHost("127.0.0.1",8000);
        if(socket->waitForConnected()){
            QMessageBox::information(this,"Information","连接成功！");
            login l;
            l.show();
            this->close();
            return;
        }
        else QMessageBox::information(this,"Warning","Connecting failed!");
}

connect1::~connect()
{
    delete ui;
}
