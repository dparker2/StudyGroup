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
void Flashcard::setFront(QString q){
    front_text = q;
}
void Flashcard::setBack(QString a){
    back_text = a;
}
QString Flashcard::getFront(){
    return front_text;
}
QString Flashcard::getBack(){
    return back_text;
}
void Flashcard::setCardNum(int num){
    cardNum = num;   // don't think this is needed
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
    if(ui->card_text_label->text() == front_text){
        ui->card_widget->setCurrentWidget(ui->card_edit);
        ui->card_textEdit->setPlainText(front_text);
        ui->current_side->setText("Front");
    }
    else{
        ui->card_widget->setCurrentWidget(ui->card_edit);
        ui->card_textEdit->setPlainText(back_text);
        ui->current_side->setText("Back");
    }

}

void Flashcard::on_set_front_btn_clicked()
{
    if(ui->current_side->text() == "Front"){
        front_text = ui->card_textEdit->toPlainText();
        ui->card_text_label->setText(front_text);
        ui->current_side->setText("Back");
        ui->set_front_btn->setText("Set Back");
        ui->card_textEdit->setPlainText(back_text);
        emit set_front(front_text, cardNum);
    }
    else{
        back_text = ui->card_textEdit->toPlainText();
        ui->card_widget->setCurrentWidget(ui->card_front);
        ui->current_side->setText("Front");
        ui->set_front_btn->setText("Set Front");
        emit set_front(back_text, cardNum);
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
        ui->card_widget->setStyleSheet("background-color: rgb(0,191,143)");
        //add back to flip
    }
    else{
        ui->card_text_label->setText(front_text);
        ui->card_widget->setStyleSheet("background-color: rgb(0,169,210)");
    }
}
