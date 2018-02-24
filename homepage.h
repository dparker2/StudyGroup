#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "groupmainpage.h"
#include "sgwidget.h"

namespace Ui {
class Homepage;
}

class Homepage : public GroupMainPage, public SGWidget
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = 0);
    ~Homepage();

private:
    Ui::Homepage *ui;
    QQueue<QPair<QString, bool>> recent_groups;
    QList<QString> favorite_groups;

    void do_work();
    void clear_recents();
    void clear_favorites();
    void insert_recents(QList<QByteArray>& groups_list);
    void insert_favorites(QList<QByteArray>& groups_list);
    void update_recents();
    void update_favorites();
};

#endif // HOMEPAGE_H
