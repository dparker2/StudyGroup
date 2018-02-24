#ifndef GROUPLISTITEM_H
#define GROUPLISTITEM_H

#include <QWidget>

namespace Ui {
class GroupListItem;
}

class GroupListItem : public QWidget
{
    Q_OBJECT

public:
    explicit GroupListItem(QWidget *parent = 0);
    ~GroupListItem();
    void set_group(QString group);
    void set_pop(QString num);
    void set_favorite(bool fav);

signals:
    void join_pressed(QString group);

private slots:
    void on_join_button_released();
    void on_fav_button_clicked(bool checked);

private:
    Ui::GroupListItem *ui;
};

#endif // GROUPLISTITEM_H
