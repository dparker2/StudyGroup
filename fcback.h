#ifndef FCBACK_H
#define FCBACK_H

#include <QWidget>
#include <QObject>

namespace Ui {
class FCBack;
}

class FCBack : public QWidget
{
    Q_OBJECT

public:
    explicit FCBack(QString group_name, QString text, int index, bool, QWidget *parent = 0);
    ~FCBack();

    void display_back();
    void display_edit_back();
    void set_index(int);
    void edit_back(QString, int, bool);

public slots:
    void save_back();

private:
    Ui::FCBack *ui;
    QString groupID;
    int current_index;
};

#endif // FCBACK_H
