#include "flashcard.h"
#include "ui_flashcard.h"
#include <QDebug>
#include <QPixmap>

Flashcard::Flashcard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
    ui->card_widget->setCurrentWidget(ui->card_front);
    ui->flip_card_btn->hide();
    ui->card_number->hide();

}

Flashcard::~Flashcard()
{
    delete ui;
}

Flashcard::Flashcard(QString q, QString a, int num){
    front_text = q;
    back_text = a;
    cardNum = num;
}
void Flashcard::setQuestion(QString q){
    front_text = q;
}
void Flashcard::setAnswer(QString a){
    back_text = a;
}
QString Flashcard::getQuestion(){
    return front_text;
}
QString Flashcard::getAnswer(){
    return back_text;
}
void Flashcard::setCardNum(int n){
    cardNum = n;   // don't think this is needed
    ui->card_number->setText("hello");
}
int Flashcard::getCardNum(){
    return cardNum;
}
void Flashcard::setEditBtn(){
    /*qDebug() << "TEST";
    QPixmap pencil(":/resources/img/exit.png");
    QIcon editBtn(pencil);
    ui->edit_card_btn->setIcon(editBtn);
    //ui->edit_card_btn->setIconSize(QSize(31,31));
    */
}

void Flashcard::on_edit_card_btn_clicked()
{
    ui->card_widget->setCurrentWidget(ui->card_edit);
    ui->card_textEdit->setPlainText(front_text);
}

void Flashcard::on_set_front_btn_clicked()
{
    if(ui->current_side->text() == "Front"){
        front_text = ui->card_textEdit->toPlainText();
        ui->card_text_label->setText(front_text);
        ui->current_side->setText("Back");
        ui->set_front_btn->setText("Set Back");
        ui->card_textEdit->setPlainText(back_text);
    }
    else{
        back_text = ui->card_textEdit->toPlainText();
        ui->card_widget->setCurrentWidget(ui->card_front);
        ui->current_side->setText("Front");
        ui->set_front_btn->setText("Set Front");
    }
    ui->flip_card_btn->show();
}

void Flashcard::on_pushButton_3_clicked()
{
    ui->card_widget->setCurrentWidget(ui->card_front);
}

void Flashcard::on_flip_card_btn_clicked()
{
    //ui->card_widget->setStyleSheet("background-image:url(:/resources/img/index_card.png); background-repeat: no-repeat");

    if(ui->card_text_label->text() == front_text){
        ui->card_text_label->setText(back_text);
        ui->card_widget->setStyleSheet("background-color: lightgreen");
    }
    else{
        ui->card_text_label->setText(front_text);
        ui->card_widget->setStyleSheet("background-color: skyblue");
    }

}
