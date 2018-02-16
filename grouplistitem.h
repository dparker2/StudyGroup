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

private:
    Ui::GroupListItem *ui;
};

#endif // GROUPLISTITEM_H
