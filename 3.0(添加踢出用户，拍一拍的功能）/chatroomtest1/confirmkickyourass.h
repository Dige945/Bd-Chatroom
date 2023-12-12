#ifndef CONFIRMKICKYOURASS_H
#define CONFIRMKICKYOURASS_H

#include <QWidget>

namespace Ui {
class confirmkickyourass;
}

class confirmkickyourass : public QWidget
{
    Q_OBJECT

public:
    explicit confirmkickyourass(QWidget *parent = nullptr);
    ~confirmkickyourass();

private:
    Ui::confirmkickyourass *ui;
};

#endif // CONFIRMKICKYOURASS_H
