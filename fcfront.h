#ifndef FCFRONT_H
#define FCFRONT_H

#include <QWidget>

namespace Ui {
class FCFront;
}

class FCFront : public QWidget
{
    Q_OBJECT

public:
    explicit FCFront(QWidget *parent = 0);
    ~FCFront();

private:
    Ui::FCFront *ui;
};

#endif // FCFRONT_H
