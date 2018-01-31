#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "groupwidget.h"
#include "server.h"
#include "user_account_check.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setStackedIndex(int index);
    void changePage(int index);
    void changePage(QWidget* new_widget);

private slots:
    void exit_settings();

    void on_logout_button_released();

    //void on_stackedWidget_inner_currentChanged(int arg1);

private:
    Ui::MainWindow *ui;
    server *my_serv;
    UserAccount *user_info;
    bool auto_login;

    QWidget* exit_settings_to;
    GroupWidget* group_widget;

    void _initialize_group();
    void _activate_group(QString& group_id);
};

#endif // MAINWINDOW_H
