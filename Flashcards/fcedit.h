#ifndef FCEDIT_H
#define FCEDIT_H

#include <QWidget>

class FCSide;

namespace Ui {
class FCEdit;
}

class FCEdit : public QWidget
{
    Q_OBJECT

public:
    explicit FCEdit(QString group_name, int index, QString label, FCSide* side, QWidget *parent = 0);
    ~FCEdit();

public slots:
    void set_side_text();
    void get_side_text();

protected:
    QString my_code;

private:
    Ui::FCEdit *ui;
    FCSide* my_side;
    QString my_group;
    int my_index;
};

#endif // FCEDIT_H
