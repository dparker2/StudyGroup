#ifndef USERLISTITEM_H
#define USERLISTITEM_H

#include <QWidget>

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserListItem(QWidget *parent = 0);
    ~UserListItem();
    void show_add_button();
    void show_remove_button();
    void hide_button();
    void show_online_status();
    void hide_online_status();
    void set_online_status(int status);
    void set_name(QString name);

private slots:
    void on_add_button_released();

private:
    Ui::UserListItem *ui;
};

#endif // USERLISTITEM_H
