#include "privatechatwidget.h"
#include "ui_privatechatwidget.h"
#include "widget.h"

Privatechatwidget::Privatechatwidget(QString username,QTcpSocket* socket,QString userid,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Privatechatwidget),
    chatusername(username),
    socket(socket),
    userid(userid)
{
    ui->setupUi(this);
    connect(ui->sendbtn,&QPushButton::clicked,this,&Privatechatwidget::on_sendbtn_clicked);

}
void Privatechatwidget::on_sendbtn_clicked(){
    QString sendstr=ui->textEdit->toPlainText();
    QString showstr = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") + ":" + '\n' + sendstr;
    ui->textBrowser->append(showstr);
    ui->textEdit->clear();
    socket->write("PRIVATECHAT"+chatusername.toUtf8()+"::"+showstr.toUtf8()+"::"+userid.toUtf8());
    socket->flush();


}
Privatechatwidget::~Privatechatwidget()
{
    delete ui;
}
