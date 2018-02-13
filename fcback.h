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
    explicit FCBack(QString groupID, QString text, int index, QWidget *parent = 0);
    ~FCBack();

    void edit_back(QString, int);
    void display_back();
    void display_edit_back();

private slots:
    void on_edit_card_btn_clicked();

    void on_set_back_btn_clicked();

private:
    Ui::FCBack *ui;
    QString groupID;
};

#endif // FCBACK_H
