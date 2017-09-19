#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_signInButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::LoginWindow *ui;
    server* my_serv;
};

#endif // LOGINWINDOW_H
