#ifndef USER_ACCOUNT_CHECK_H
#define USER_ACCOUNT_CHECK_H

#include <QMainWindow>
#include <regex>


class UserAccount : public QMainWindow
{
    Q_OBJECT
public:
    explicit UserAccount(QWidget *parent = nullptr);
    bool emailValidation (QString emailStr, QString &error_msg);
    bool usernameValidation(QString usernameStr, QString &error_msg);
    bool passwordValidtion(QString passwordStr, QString &error_msg);

    void setUsername(QString usernameStr);
    void setEmail(QString emailStr);
    void setPassword(QString passwordStr);

    QString getUsername();
    QString getEmail();
    QString getPassword();

    void set_info_complete(int i, bool set);
    bool get_info_complete(int i);


signals:

public slots:

private:
    QString acct_username;
    QString acct_email;
    QString acct_password;
    bool info_complete[];   // stores 3 true values(username,email,password)

};

#endif // USER_ACCOUNT_CHECK_H
