#ifndef CONNECT1_H
#define CONNECT1_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>


namespace Ui {
class connect1;
}

class connect1 : public QWidget
{
    Q_OBJECT

public:
    explicit connect1(QWidget *parent = nullptr);
    ~connect1();
    void connectbutton();
private:
    Ui::connect1 *ui;
};

#endif // CONNECT1_H
