#ifndef JOINGROUPPAGE_H
#define JOINGROUPPAGE_H

#include <QWidget>
#include "groupmainpage.h"

namespace Ui {
class JoinGroupPage;
}

class JoinGroupPage : public GroupMainPage
{
    Q_OBJECT

public:
    explicit JoinGroupPage(QWidget *parent = 0);
    ~JoinGroupPage();

private slots:
    void on_join_group_button_released();

private:
    Ui::JoinGroupPage *ui;
};

#endif // JOINGROUPPAGE_H
