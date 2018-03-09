#ifndef RESETPASSWORD_H
#define RESETPASSWORD_H

#include <QWidget>

namespace Ui {
class ResetPassword;
}

class ResetPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPassword(QWidget *parent = 0);
    ~ResetPassword();

private:
    Ui::ResetPassword *ui;
};

#endif // RESETPASSWORD_H
