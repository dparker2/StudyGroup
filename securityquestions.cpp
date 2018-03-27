#include "securityquestions.h"
#include "ui_securityquestions.h"
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>

SecurityQuestions::SecurityQuestions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityQuestions)
{
    ui->setupUi(this);
}

bool SecurityQuestions::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
            this->focusNextChild();
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

SecurityQuestions::~SecurityQuestions()
{
    delete ui;
}
void SecurityQuestions::REQQ(QList<QByteArray> message)
{
    comboBox_ptrs_to_list();
    init_question_list(message);
    init_comboBox_list();
}

void SecurityQuestions::comboBox_ptrs_to_list()
{
    QComboBox* comboBox;
    for(int i = 0; i < 3; i++){
        comboBox = parentWidget()->findChild<QComboBox*>("comboBox_q" + QString::number(i+1));
        comboBox->installEventFilter(this);
        comboBoxes.push_back(comboBox);
    }
}

void SecurityQuestions::init_question_list(QList<QByteArray> message)
{
    default_questions.push_back("Choose Security Question");
    for(int i = 0; i < message.size(); i++){
        default_questions.push_back(QString(message[i]).replace('_', ' '));
        selected_questions.push_back(QString(message[0]).replace('_', ' '));
    }
    default_questions.push_back("Create Custom Question");
}

void SecurityQuestions::init_comboBox_list()
{
    for(int i = 0; i < comboBoxes.size(); i++)
    {
        if(comboBoxes[i]->lineEdit() != 0){
            selected_questions[i] = comboBoxes[i]->lineEdit()->text();
        }
        comboBoxes[i]->clear();
        for(int j = 0; j < default_questions.size(); j++){
            comboBoxes[i]->addItem(default_questions[j]);
        }
    }
}

void SecurityQuestions::set_custom_question(QComboBox* current_comboBox)
{
    current_comboBox->setEditable(true);
    current_comboBox->lineEdit()->clear();
    set_list_stylesheet(current_comboBox);
}
void SecurityQuestions::set_list_stylesheet(QComboBox* combobox){
    combobox->setStyleSheet("QComboBox{"
                                        "background-color: #545454; "
                                        "color:white; border-style:none; "
                                        "font-size: 16px"
                                    "}");
}

void SecurityQuestions::update_selected_questions()
{
    for(int i = 0; i < comboBoxes.count(); i++){
        selected_questions[i] = comboBoxes[i]->currentText();
    }
}
void SecurityQuestions::update_comboBoxes()
{
    update_selected_questions();
    init_comboBox_list();

    int index;
    for(int i = 0; i < comboBoxes.size(); i++)
    {
        for(int j = 0; j < selected_questions.size(); j++)
        {
            if(i != j && selected_questions[j] != default_questions[0] && selected_questions[j] != default_questions[default_questions.size()-1]){    // can't be the first index, or last
                index = comboBoxes[i]->findText(selected_questions[j]);
                comboBoxes[i]->removeItem(index);      // removes current selected question from the other combobox lists
            }            
        }
        if(comboBoxes[i]->lineEdit() != 0){
            comboBoxes[i]->lineEdit()->setText(selected_questions[i]);
        }
        else{
            comboBoxes[i]->setCurrentIndex(comboBoxes[i]->findText(selected_questions[i]));
        }
    }
}
void SecurityQuestions::on_comboBox_q1_activated(int index)
{
    set_list_stylesheet(comboBoxes[0]);
    comboBoxes[0]->setEditable(false);

    if(index == comboBoxes[0]->count()-1){
        comboBoxes[0]->setEditable(true);
        comboBoxes[0]->lineEdit()->clear();
        set_list_stylesheet(comboBoxes[0]);
    }
        update_comboBoxes();
}

void SecurityQuestions::on_comboBox_q1_currentTextChanged(const QString &custom_q)
{
    if(!custom_q.isEmpty() && custom_q != default_questions[0] && custom_q != comboBoxes[0]->count()-1){
        selected_questions[0].clear();
        selected_questions[0] = custom_q;
    }
    customQ_flag = true;
}
void SecurityQuestions::on_comboBox_q2_activated(int index)
{
    set_list_stylesheet(comboBoxes[1]);
    comboBoxes[1]->setEditable(false);

    if(index == comboBoxes[1]->count()-1){
        comboBoxes[1]->setEditable(true);
        comboBoxes[1]->lineEdit()->clear();
        set_list_stylesheet(comboBoxes[1]);
    }
    update_comboBoxes();
}
void SecurityQuestions::on_comboBox_q2_currentTextChanged(const QString &custom_q)
{
    int size = default_questions.size();
    if(!custom_q.isEmpty() && custom_q != default_questions[0] && custom_q != comboBoxes[1]->count()-1){
        selected_questions[1].clear();
        selected_questions[1] = custom_q;
    }
    customQ_flag = true;
}
void SecurityQuestions::on_comboBox_q3_activated(int index)
{
    set_list_stylesheet(comboBoxes[2]);
    comboBoxes[2]->setEditable(false);

    if(index == comboBoxes[2]->count()-1){
        comboBoxes[2]->setEditable(true);
        comboBoxes[2]->lineEdit()->clear();
        set_list_stylesheet(comboBoxes[2]);
    }
    update_comboBoxes();
}
void SecurityQuestions::on_comboBox_q3_editTextChanged(const QString &custom_q)
{
    int size = default_questions.size();
    if(!custom_q.isEmpty() && custom_q != default_questions[0] && custom_q != comboBoxes[2]->count()-1){
        selected_questions[2].clear();
        selected_questions[2] = custom_q;
    }
    customQ_flag = true;
}

bool SecurityQuestions::ready_to_send(QString &security_question_msg, QString &security_answer_msg)
{
    if(!questions_ready() || !answers_ready()){
        return false;
    }
    for(int i = 0; i < selected_questions.size(); i++){
        QLineEdit* answer = parentWidget()->findChild<QLineEdit*>("lineEdit_answer" + QString::number(i+1));
        security_answer_msg += answer->text().replace(' ', '_') + " ";
        security_question_msg += selected_questions[i].replace(' ', '_') + " ";
    }

    return false;
}
bool SecurityQuestions::questions_ready()
{
    update_selected_questions();

    for(int i = 0; i < selected_questions.size(); i++){
        qDebug() << "Question: " << selected_questions[i];
        if(selected_questions[i] == default_questions[0]){
            QMessageBox error_box;
            error_box.information(0, "Error Message", "Please make a selection for question " + QString::number(i+1));
            return false;
        }
    }
    return true;
}
bool SecurityQuestions::answers_ready()
{
    for(int i = 0; i < comboBoxes.size(); i++){
        QLineEdit* answer = parentWidget()->findChild<QLineEdit*>("lineEdit_answer" + QString::number(i+1));
        if(answer->text().isEmpty()){
            QMessageBox error_box;
            error_box.information(0, "Error Message", "Please enter an answer for question " + QString::number((i+1)));
            return false;
        }
    }
    return true;
}

