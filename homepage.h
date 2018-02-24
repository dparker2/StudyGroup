#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "groupmainpage.h"
#include "sgwidget.h"

namespace Ui {
class Homepage;
}

class Homepage : public GroupMainPage
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = 0);
    ~Homepage();
    set_active(bool);

private:
    Ui::Homepage *ui;
    bool is_active;

    void do_work();
    void clear_recents();
    void clear_favorites();
    void update_recents(QList<QByteArray>& groups_list);
    void update_favorites(QList<QByteArray>& groups_list);

private slots:
    void on_join_group(QString group);
};

#endif // HOMEPAGE_H
