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
    void setStackedIndex(unsigned index);
    void changePage(unsigned index);
    void newPage(QWidget* new_widget, QString title);
    void removeCurrentPage();

private slots:
    void on_settings_button_released();
    void on_leave_button_released();
    void on_logout_button_released();

    //void on_stackedWidget_inner_currentChanged(int arg1);

private:
    Ui::MainWindow *ui;
    server *my_serv;
    UserAccount *user_info;
    bool auto_login;

    QWidget* exit_settings_to;
    GroupWidget* group_widget;

    // New stuff
    QList<QPushButton*> left_buttons_list;
    //

    void _initialize_group();
    void _activate_group(QString& group_id);

    // New stuff
    void set_active_button(unsigned index);
    void append_left_button(QString title);
    QPushButton* copy_button(QPushButton* button);
    void remove_button(unsigned index);
    void show_leave_button(bool show);
    //
};

#endif // MAINWINDOW_H
