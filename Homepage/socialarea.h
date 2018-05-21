#ifndef SOCIALAREA_H
#define SOCIALAREA_H

#include "sgwidget.h"


namespace Ui {
class SocialArea;
}

class SocialArea : public SGWidget
{
    Q_OBJECT

public:
    explicit SocialArea(QWidget *parent = 0, QString name = "socialarea");
    ~SocialArea();

private slots:
    void do_work();
    void on_username_search_textEdited(const QString &arg1);

private:
    void clear_list(QLayout* layout);
    void put_into_list(QLayout* layout, QList<QByteArray> items);

    Ui::SocialArea *ui;
};

#endif // SOCIALAREA_H
