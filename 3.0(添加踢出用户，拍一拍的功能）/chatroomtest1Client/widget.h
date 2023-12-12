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
    //实现拍一拍功能
    void pat(const QString&);
    void on_pat_clicked();
    //实现右键打开菜单的操作
    void showContextMenu(const QPoint &pos);

    //实现加粗与倾斜功能
    void on_jiacu_clicked();
    void on_qingxie_clicked();

    //字体更换
    void on_fontComboBox_currentFontChanged(const QFont &font);
    //接受用户选择的字体，引用的话效率更快

    //字体大小的修改
    void on_FontSizeChanged(const QString&);

    //私聊功能的实现
    void startprivatechat(QTableWidgetItem*);
    QString userid="username";//用户名
private:
    Ui::Widget *ui;
    QTcpSocket *socket;
    bool connectstatement=false;
    //当前选中的用户
    QString currentRightClickedUsername;
    QMenu* contextMenu;

};
#endif // WIDGET_H
