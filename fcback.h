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
    explicit FCBack(QString, QWidget *parent = 0);
    ~FCBack();

    void edit_back(QString, bool);
    void display_back();
    void display_edit_back();

private:
    Ui::FCBack *ui;
};

#endif // FCBACK_H
