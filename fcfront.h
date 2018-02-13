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
    explicit FCFront(QString groupID, QString text, int index, QWidget *parent);
    ~FCFront();

    void edit_front(QString, int);
    void display_front();
    void display_edit_front();


private slots:
    void on_edit_card_btn_clicked();

    void on_set_front_btn_clicked();

private:
    Ui::FCFront *ui;
    QString groupID;
};

#endif // FCFRONT_H
