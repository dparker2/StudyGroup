#ifndef SECURITYQUESTIONS_H
#define SECURITYQUESTIONS_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class SecurityQuestions;
}

class SecurityQuestions : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityQuestions(QWidget *parent = 0);
    ~SecurityQuestions();
    void set_questions(QList<QByteArray> message);
    void REQQ(QList<QByteArray> message);

private slots:

    void on_save_question_btn_clicked();
    void on_comboBox_q1_activated(int index);
    void on_comboBox_q2_activated(int index);
    void on_comboBox_q3_activated(int index);
    void update_question_list(int current_index, QComboBox* current_comboBox);

private:
    Ui::SecurityQuestions *ui;
    bool customQ_flag;
    QList<QLineEdit*> custom_questions;
    QList<QString> questions;
    QList<QComboBox*> question_comboBox;
};

#endif // SECURITYQUESTIONS_H
