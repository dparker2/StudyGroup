#ifndef FCBACK_H
#define FCBACK_H

#include <QWidget>

namespace Ui {
class FCBack;
}

class FCBack : public QWidget
{
    Q_OBJECT

public:
    explicit FCBack(QWidget *parent = 0);
    ~FCBack();

private:
    Ui::FCBack *ui;
};

#endif // FCBACK_H