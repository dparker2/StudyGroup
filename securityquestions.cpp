#include "securityquestions.h"
#include "ui_securityquestions.h"
#include <QDebug>

SecurityQuestions::SecurityQuestions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityQuestions)
{
    ui->setupUi(this);
}

SecurityQuestions::~SecurityQuestions()
{
    delete ui;
}

void SecurityQuestions::REQQ(QList<QByteArray> message)
{
    QComboBox* comboBox;
    for(int i = 0; i < 3; i++){
        comboBox = parentWidget()->findChild<QComboBox*>("comboBox_q" + QString::number(i+1));
        question_comboBox.push_back(comboBox);
    }

    questions.push_back("Choose Security Question");
    for(int i = 0; i < message.size(); i++)
    {
        questions.push_back(QString(message[i]).replace('_', ' '));
        custom_questions.push_back(nullptr);
    }
     questions.push_back("Create Custom Question");

     qDebug() << "QUESTOINS : " << questions;
    for(int i = 0; i < question_comboBox.size(); i++)
    {
        for(int j = 0; j < questions.size(); j++)
        {
            question_comboBox[i]->addItem(questions[j]);
        }
    }
}

void SecurityQuestions::set_questions(QList<QByteArray> message)
{

}

/*
 * Send security question and answers to server
*****/
void SecurityQuestions::on_save_question_btn_clicked()
{
    // Replaces all  spaces with underscores before sending to server
    QString questions_to_send, answers_to_send;

    for(int i = 0; i < custom_questions.size(); i++){
        if(custom_questions[i] != nullptr)
        {
            questions[i+1] = custom_questions[i]->text();
        }
    }
    for(int i = 1; i < questions.size()-1; i++){
        questions_to_send += questions[i].replace(' ', '_') + " ";
    }

    answers_to_send = ui->lineEdit_answer1->text().replace(' ', '_') + " " + ui->lineEdit_answer2->text().replace(' ', '_') + " " + ui->lineEdit_answer3->text().replace(' ', '_');


    // If there exists a custom question send all questions to server
    /*
    if(customQ_flag)
    {
        server::send(server::SECURITYQ_CUSTOM+username + " " + questions_to_send);
    }
    server::send(server::SECURITY_ANSWERS+username + " " + answers_to_send);  // Answers get sent regardless
    */
}

/*
 *  When user chooses custom question the custom flag is set to true
 *  and the current combobox is hidden and replaced with a QLineEdit
********/
void SecurityQuestions::on_comboBox_q1_activated(int index)
{
    //customQ_flag = false;
    if(index == question_comboBox[0]->count()-1)  // Custom question is picked
    {
        custom_questions[0] = new QLineEdit();
        custom_questions[0]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;  // Just in case I need the flag for something not sure yet
        ui->comboBox_q1->hide();
    }
    else{
        update_question_list(index, ui->comboBox_q1);
    }
}
// HAVE TO RESTRUCTURE THIS
void SecurityQuestions::update_question_list(int current_index, QComboBox* current_comboBox)
{
    QList<QComboBox*> other_comboBox;           // pointer for combobox not currently selected
    QList<QString> other_selected_question;     // selected questions from the other comboboxes
    QList<int> other_selected_index;            // indexes of selected questions from other combo boxes

    for(int i = 0; i < question_comboBox.size(); i++)   // loops the number of comboboxes in UI (3)
    {
       if(question_comboBox[i] != current_comboBox)     // catches the other two comboboxes
       {
           other_comboBox.push_back(question_comboBox[i]);
       }
    }
    QString current_question = current_comboBox->currentText();   // the one currently selected

    for(int j = 0; j < other_comboBox.size(); j++){
        other_selected_question.push_back(other_comboBox[j]->currentText());   // pushes back the questions selected at two other comboboxes
        other_selected_index.push_back(other_comboBox[j]->currentIndex());
        other_comboBox[j]->clear();                 // clears all questions
        other_comboBox[j]->addItems(questions);     // adds them all back in, ready to remove the ones that are currently selected
    }

    int j = 1;
    for(int i = 0; i < other_comboBox.size(); i++)
    {
        // removes the currently selected question from the two other comboxes
        if(current_index != 0){
            other_comboBox[i]->removeItem(other_comboBox[i]->findText(current_question));
        }
        if(other_selected_index[j] != 0 && other_selected_index[j] != questions.size()-1){
            other_comboBox[i]->removeItem(other_comboBox[i]->findText(other_selected_question[j]));
        }
        j--;
    }
    // Finding the new index of selected questions and setting that index to be selected
    int new_index1 = other_comboBox[0]->findText(other_selected_question[0]);
    int new_index2 = other_comboBox[1]->findText(other_selected_question[1]);
    other_comboBox[0]->setCurrentIndex(new_index1);
    other_comboBox[1]->setCurrentIndex(new_index2);
}
void SecurityQuestions::on_comboBox_q2_activated(int index)
{
    if(index == question_comboBox[1]->count()-1)
    {
        custom_questions[1] = new QLineEdit();
        custom_questions[1]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;
        ui->comboBox_q2->hide();
    }
    else{
        update_question_list(index, ui->comboBox_q2);
    }

}
void SecurityQuestions::on_comboBox_q3_activated(int index)
{
    if(index == question_comboBox[2]->count()-1)
    {
        custom_questions[2] = new QLineEdit();
        custom_questions[2]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;
        ui->comboBox_q3->hide();
    }
    else
    {
        update_question_list(index, ui->comboBox_q3);
    }
}
