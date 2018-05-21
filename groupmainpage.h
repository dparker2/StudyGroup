#ifndef GROUPMAINPAGE_H
#define GROUPMAINPAGE_H

#include "sgwidget.h"

class GroupMainPage : public SGWidget
{
    Q_OBJECT

public:
    explicit GroupMainPage(QString name, QWidget *parent = 0);

signals:
    void group_joined(QWidget*, QString group_id);

protected:
    virtual void do_work() = 0;
    bool create_group(QString group);
    bool join_group(QString group);
};

#endif // GROUPMAINPAGE_H
