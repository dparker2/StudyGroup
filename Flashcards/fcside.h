#ifndef FCSIDE_H
#define FCSIDE_H

#include <QWidget>

namespace Ui {
class FCSide;
}

class FCSide : public QWidget
{
    Q_OBJECT

public:
    explicit FCSide(QWidget *parent = 0);
    virtual ~FCSide();
    void set_text(QString&);
    QString get_text();

private:
    Ui::FCSide *ui;
    QString groupID;
    int index;
};

#endif // FCSIDE_H
