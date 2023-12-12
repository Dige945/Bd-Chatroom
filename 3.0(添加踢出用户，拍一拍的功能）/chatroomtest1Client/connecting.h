ifndef CONNECTING_H
#define CONNECTING_H

#include <QWidget>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

namespace Ui {
class connecting;
}

class connecting : public QWidget
{
    Q_OBJECT

public:
    explicit connecting(QWidget *parent = nullptr);
    ~connecting();
    void connectbutton();

private:
    Ui::connecting *ui;
};

#endif // CONNECTING_H
