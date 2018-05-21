#ifndef CREATEGROUPPAGE_H
#define CREATEGROUPPAGE_H

#include <QWidget>
#include "groupmainpage.h"

namespace Ui {
class CreateGroupPage;
}

class CreateGroupPage : public GroupMainPage
{
    Q_OBJECT

public:
    explicit CreateGroupPage(QWidget *parent = 0);
    ~CreateGroupPage();

private slots:
    void on_create_group_button_released();

private:
    void do_work();

    Ui::CreateGroupPage *ui;
};

#endif // CREATEGROUPPAGE_H
