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
    //ui->bottom_buttons->setCurrentIndex(0);
    ui->card_widget->setCurrentIndex(0);

}
Flashcard::~Flashcard()
{
    delete ui;
}

Flashcard::Flashcard(QString name, QString text1, QString text2, int num, QWidget *parent) :
    QWidget(parent),

    ui(new Ui::Flashcard)
{
    ui->setupUi(this);

    if(text1 != nullptr)
        front_text = text1;
    cardNum = num;
    if(text2 != nullptr)
        back_text = text2;


    // Front/Back widgets
    groupID = name;
    front = new FCFront(groupID, front_text, num, parent);
    back = new FCBack(groupID, back_text, num, parent);
    ui->card_widget->addWidget(front);
    ui->card_widget->addWidget(back);

    /*
    // Defaults buttons
    ui->bottom_buttons->setCurrentIndex(0);
    ui->flip_card_btn->show();
    ui->card_index->setText(QString::number(cardNum+1));
    current_side = false;
    */
}

void Flashcard::setFront(QString q){
    //front_text = q;
    //front->edit_front(groupID, front_text, cardNum);
}
void Flashcard::setBack(QString a){
    //back_text = a;
    //back->edit_back(back_text, false);
}
QString Flashcard::getFront(){
    //return front_text;
}

QString Flashcard::getBack(){
    //return back_text;
}
void Flashcard::setCardNum(int num){
    cardNum = num;
    //ui->card_index->setText(QString::number(num));
}
int Flashcard::getCardNum(){
    //return cardNum;
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
/*
    if(current_side){
        back->edit_back(back_text, true);    // true displays edit side
        ui->card_widget->setCurrentWidget(back);
        ui->bottom_buttons->setCurrentIndex(2);
        //emit check_set_card(this, back_text, cardNum, current_side);
    }
    else{
        front->edit_front(front_text, true);// true displays edit side
        ui->card_widget->setCurrentWidget(front);
        ui->bottom_buttons->setCurrentIndex(1);
        //emit check_set_card(this, front_text, cardNum, current_side);
    }
*/
}

void Flashcard::on_flip_card_btn_clicked()
{
    if(current_side){
        front->display_front();
        ui->card_widget->setCurrentWidget(front);
        ui->frame->setStyleSheet("background-color: rgba(2, 128, 173)");
    }
    else{
        back->display_back();
        ui->card_widget->setCurrentWidget(back);
        ui->frame->setStyleSheet("background-color: rgba(0, 190, 144)");
    }
    current_side ^= true; // testing sides (xor)
}

// Buttons confirm text entered into card
void Flashcard::on_set_front_btn_clicked()
{
    current_side = true;
    on_edit_card_btn_clicked();
}

void Flashcard::on_set_back_btn_clicked()
{
/*
    current_side = false;
    front->display_front();
    ui->card_widget->setCurrentWidget(front);
    //ui->bottom_buttons->setCurrentIndex(0);
    ui->frame->setStyleSheet("background-color: rgba(2, 128, 173)");
*/
}

void Flashcard::on_cancel_btn_clicked()
{
    ui->card_widget->setCurrentIndex(0);
    //ui->bottom_buttons->setCurrentIndex(0);
    ui->flip_card_btn->show();
}



