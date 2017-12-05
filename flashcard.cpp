#include "flashcard.h"
#include "ui_flashcard.h"
#include <QDebug>
#include <QPixmap>

Flashcard::Flashcard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
    ui->flip_card_btn->hide();
    cardNum = -1;
    ui->bottom_buttons->setCurrentIndex(0);
    ui->card_widget->setCurrentIndex(0);
}
Flashcard::~Flashcard()
{
    delete ui;
}

Flashcard::Flashcard(QString text1, QString text2, int num, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
    if(text1 != nullptr)
        front_text = text1;
    cardNum = num;
    if(text2 != nullptr)
        back_text = text2;
    ui->back_label->setText(back_text);
    ui->front_label->setText(front_text);
    ui->card_widget->setCurrentIndex(0);
    ui->bottom_buttons->setCurrentIndex(0);
    ui->flip_card_btn->show();
    ui->card_index->setText(QString::number(cardNum+1));
}
void Flashcard::setFront(QString q){
    front_text = q;
    ui->front_label->setText(front_text);
}
void Flashcard::setBack(QString a){
    back_text = a;
    ui->back_label->setText(back_text);
    qDebug() << "I AM HERE";
}
QString Flashcard::getFront(){
    return front_text;
}
QString Flashcard::getBack(){
    return back_text;
}
void Flashcard::setCardNum(int num){
    cardNum = num;   // don't think this is needed
    ui->card_index->setText(QString::number(num));
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

void Flashcard::emit_init_signal()
{
    QString _s = "";
    emit check_set_card(this, _s, cardNum, 0);
}

/**
 * PRIVATE/SLOTS
 */

void Flashcard::on_edit_card_btn_clicked()
{
    ui->bottom_buttons->setCurrentIndex(1);
    if(ui->card_widget->currentIndex() == 0){
        ui->card_textEdit->setPlainText(front_text);
        ui->current_side->setText("Front");
    }
    else{
        ui->card_textEdit->setPlainText(back_text);
        ui->current_side->setText("Back");
    }
    ui->card_widget->setCurrentIndex(2);

}

void Flashcard::on_set_front_btn_clicked()
{
    if(ui->current_side->text() == "Front"){
        front_text = ui->card_textEdit->toPlainText();
        ui->front_label->setText(front_text);
        ui->current_side->setText("Back");
        ui->set_front_btn->setText("Set Back");
        ui->card_textEdit->setPlainText(back_text);
        QString front = front_text;
        int side = 0;
        emit check_set_card(this, front, cardNum, side);
        //ui->card_index->setText(QString::number(cardNum+1));
        qDebug() << "cardNum: " << cardNum;
    }
    else {
        back_text = ui->card_textEdit->toPlainText();
        ui->back_label->setText(back_text);
       // ui->card_widget->setCurrentWidget(0);
        ui->current_side->setText("Front");
        ui->set_front_btn->setText("Set Front");
        ui->card_widget->setCurrentIndex(0);
        ui->bottom_buttons->setCurrentIndex(0);
        ui->flip_card_btn->show();
        emit check_set_card(this, back_text, cardNum, 1);
        //ui->card_index->setText(QString::number(cardNum));
    }

}

void Flashcard::on_cancel_btn_clicked()
{
    ui->card_widget->setCurrentIndex(0);
    ui->bottom_buttons->setCurrentIndex(0);
    ui->flip_card_btn->show();
}

void Flashcard::on_flip_card_btn_clicked()
{
    //ui->card_widget->setStyleSheet("background-image:url(:/resources/img/index_card.png); background-repeat: no-repeat");

    if(ui->card_widget->currentIndex() == 0){
        ui->card_widget->setCurrentIndex(1);
        ui->frame->setStyleSheet("background-color: rgba(0, 190, 144)");
    }
    else{
        qDebug() << "FLIP TO FONT" << endl;
        ui->card_widget->setCurrentIndex(0);
        ui->frame->setStyleSheet("background-color: rgba(2, 128, 173)");
    }
}
