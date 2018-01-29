#ifndef CREATEGROUPPAGE_H
#define CREATEGROUPPAGE_H

#include <QWidget>

namespace Ui {
class CreateGroupPage;
}

class CreateGroupPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroupPage(QWidget *parent = 0);
    ~CreateGroupPage();

private slots:
    void on_create_group_button_released();

private:
    Ui::CreateGroupPage *ui;
};

#endif // CREATEGROUPPAGE_H
