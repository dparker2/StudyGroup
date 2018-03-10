#ifndef SECURITYQUESTIONS_H
#define SECURITYQUESTIONS_H

#include <QWidget>

namespace Ui {
class SecurityQuestions;
}

class SecurityQuestions : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityQuestions(QWidget *parent = 0);
    ~SecurityQuestions();

private:
    Ui::SecurityQuestions *ui;
};

#endif // SECURITYQUESTIONS_H
