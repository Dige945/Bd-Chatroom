#include "privatechatwindow.h"
#include "ui_privatechatwindow.h"
#include "mainwindow.h"

PrivateChatWindow::PrivateChatWindow(QTcpSocket* socket,MainWindow* server,QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::PrivateChatWindow),
    privatesocket(socket),
    server(server)
{
    ui->setupUi(this);
}

void PrivateChatWindow::on_sendbtn_clicked(){
    QString sendstr = ui->textEdit->toPlainText();
    QString showstr = QDateTime::currentDateTime().toString("dddd.yyyy.MM.dd HH:mm:ss") + ":" + '\n' + sendstr;
    ui->textBrowser->append(showstr);
    privatesocket->write("SERVERMESSAGE:" + showstr.toUtf8());
    ui->textEdit->clear();

}
PrivateChatWindow::~PrivateChatWindow()
{
    delete ui;
}
