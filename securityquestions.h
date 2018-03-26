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
    void set_custom_question(QComboBox* current_comboBox);
    void REQQ(QList<QByteArray> message);

private slots:
    void comboBox_ptrs_to_list();
    void init_question_list(QList<QByteArray> message);
    void init_comboBox_list();
    void on_comboBox_q1_activated(int index);
    void on_comboBox_q2_activated(int index);
    void on_comboBox_q3_activated(int index);
    void update_selected_questions(QList<QString> &selected_questions);
    void update_comboBoxes(QComboBox* current_combobox, bool custom);

    void set_list_stylesheet(QComboBox* combobox);

    void on_comboBox_q1_currentTextChanged(const QString &arg1);


    bool eventFilter(QObject* obj, QEvent* event);

    void on_comboBox_q2_currentTextChanged(const QString &arg1);

    void on_comboBox_q3_editTextChanged(const QString &arg1);

private:
    Ui::SecurityQuestions *ui;
    bool customQ_flag;
    QList<QString> custom_questions;
    QList<QString> default_questions;
    QList<QComboBox*> comboBoxes;
};

#endif // SECURITYQUESTIONS_H
