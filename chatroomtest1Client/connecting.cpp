#include "connecting.h"
#include "ui_connecting.h"
#include "login.h"
#include "widget.h"

connecting::connecting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connecting)
{
    ui->setupUi(this);
    connect(ui->connectButton,&QPushButton::clicked,this,&connecting::connectbutton);

}

void connecting::connectbutton(){
    QTcpSocket *socket = new QTcpSocket;
        socket->connectToHost("127.0.0.1",8000);
        if(socket->waitForConnected()){
            QMessageBox::information(this,"Information","连接成功！");
            login *l = new login;
            l->show();
            socket->close();
            this->close();

        }
        else QMessageBox::information(this,"Warning","Connecting failed!");
}
connecting::~connecting()
{
    delete ui;
}
