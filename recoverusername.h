#ifndef RECOVERUSERNAME_H
#define RECOVERUSERNAME_H

#include "sgwidget.h"
#include <QWidget>

namespace Ui {
class RecoverUsername;
}

class RecoverUsername : public SGWidget
{
    Q_OBJECT

public:
    explicit RecoverUsername(QString name, QWidget *parent = 0);
    ~RecoverUsername();

    void clear_text();

private slots:
    void do_work();
    void RUSR(QString email);
    void set_invalid_icon();
    void on_recover_username_btn_clicked();

private:
    Ui::RecoverUsername *ui;
};

#endif // RECOVERUSERNAME_H
