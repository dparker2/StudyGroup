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
}
Flashcard::~Flashcard()
{
    delete ui;
}

Flashcard::Flashcard(QString groupID, QString text1, QString text2, int num, bool send_card, QWidget *parent) :
    QWidget(parent),

    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
    back_side = 0;

    // Initialize Front/Back widgets
    front = new FCFront(groupID, text1, num, send_card, parent);
    back = new FCBack(groupID, text2, num, send_card, parent);
    ui->card_widget->addWidget(front);
    ui->card_widget->addWidget(back);
    ui->card_widget->setCurrentWidget(front);

    ui->card_index->setText(QString::number(num+1));    // Set flashcard index in ui
}

/*
 * Set front/back of card
 * send_card determines whether card needs to be sent to server
*/
void Flashcard::setFront(QString text, int index, bool send_card)
{
    front->edit_front(text, index, send_card);
}
void Flashcard::setBack(QString text, int index, bool send_card)
{
    back->edit_back(text, index, send_card);
}
void Flashcard::set_card_num(int num)
{
    cardNum = num;
    ui->card_index->setText(QString::number(num));
}

/**
 * PRIVATE/SLOTS
 */

/*
 * Displays the editing side of the card
***/
void Flashcard::on_edit_card_btn_clicked()
{
    ui->card_widget->setCurrentWidget(ui->edit_view);
    qDebug() << ui->card_widget->count();
    ui->card_widget->removeWidget(front);
    qDebug() << ui->card_widget->count();
    qDebug() << front->layout();
    ui->cards_edit->addWidget(front);
    delete front->layout();
    front->setLayout(ui->cards_edit);
    qDebug() << front->layout();

    front->display_edit_front();
    ui->card_widget->removeWidget(back);
    ui->cards_edit->addWidget(back);
    back->display_edit_back();
}

/*
 * Flips between front and back side of card
 * using xor to do this, need to come up with something different maybe
***/
void Flashcard::on_flip_card_btn_clicked()
{
    if(back_side){
        front->display_front();
        ui->card_widget->setCurrentWidget(front);
    }
    else{
        back->display_back();
        ui->card_widget->setCurrentWidget(back);
    }
    back_side = !back_side;
}

void Flashcard::on_card_widget_currentChanged(int arg1)
{
    //this->setStyleSheet('background-color: rgb(')
}

void Flashcard::on_save_button_released()
{
    //ui->cards_edit->removeWidget(front);
    //ui->cards_edit->removeWidget(back);
    ui->card_widget->addWidget(front);
    ui->card_widget->addWidget(back);
    ui->card_widget->setCurrentWidget(front);
    front->save_front();
    back->save_back();
}
