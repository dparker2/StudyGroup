#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace Ui {
class Homepage;
}

class Homepage : public QWidget
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = 0);
    ~Homepage();

private:
    Ui::Homepage *ui;
};

#endif // HOMEPAGE_H
