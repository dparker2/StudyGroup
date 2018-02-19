#ifndef FCFRONT_H
#define FCFRONT_H

#include <QWidget>
#include <QObject>

namespace Ui {
class FCFront;
}

class FCFront : public QWidget
{
    Q_OBJECT

public:
    explicit FCFront(QString group_name, QString text, int index, bool is_new, QWidget *parent);
    ~FCFront();

    void display_front();
    void display_edit_front();
    void set_index(int);
    void edit_front(QString, int, bool);

private slots:
    void on_set_front_btn_clicked();

private:
    Ui::FCFront *ui;
    QString groupID;
    int current_index;
};

#endif // FCFRONT_H
