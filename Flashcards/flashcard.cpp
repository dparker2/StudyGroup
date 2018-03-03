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

Flashcard::Flashcard(QString groupID, QString front_text, QString back_text, int index, bool send_card, QWidget *parent) :
    QWidget(parent),

    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
    back_side = 0;

    // Initialize Front/Back widgets
    ui->front_side->set_text(front_text);
    fc_front_edit = new FCFrontEdit(groupID, index, ui->front_side);
    ui->edits->addWidget(fc_front_edit);

    ui->back_side->set_text(back_text);
    fc_back_edit = new FCBackEdit(groupID, index, ui->back_side);
    ui->edits->addWidget(fc_back_edit);

    if(send_card) {
        fc_front_edit->set_side_text();
    }

    show_front();

    ui->card_index->setText(QString::number(index+1));    // Set flashcard index in ui
}

/*
 * Set front/back of card
 * send_card determines whether card needs to be sent to server
*/
void Flashcard::setFront(QString text, int index, bool send_card)
{
    ui->front_side->set_text(text);
}
void Flashcard::setBack(QString text, int index, bool send_card)
{
    ui->back_side->set_text(text);
}
void Flashcard::set_card_num(int num)
{
    cardNum = num;
    ui->card_index->setText(QString::number(num));
}

void Flashcard::show_front()
{
    ui->stackedOuter->setCurrentWidget(ui->card_view);
    ui->stackedInner->setCurrentWidget(ui->front_side);
}

/**
 * PRIVATE/SLOTS
 */

/*
 * Displays the editing side of the card
***/
void Flashcard::on_edit_card_btn_clicked()
{
    ui->stackedOuter->setCurrentWidget(ui->edit_view);
    fc_front_edit->get_side_text();
    fc_back_edit->get_side_text();
}

/*
 * Flips between front and back side of card
 * using xor to do this, need to come up with something different maybe
***/
void Flashcard::on_flip_card_btn_clicked()
{
    if(ui->stackedInner->currentWidget() == ui->front_side){
        ui->stackedInner->setCurrentWidget(ui->back_side);
    }
    else{
        ui->stackedInner->setCurrentWidget(ui->front_side);
    }
}

void Flashcard::on_card_widget_currentChanged(int arg1)
{
    //this->setStyleSheet('background-color: rgb(')
}

void Flashcard::on_save_button_released()
{
    fc_front_edit->set_side_text();
    fc_back_edit->set_side_text();
    ui->stackedOuter->setCurrentWidget(ui->card_view);
    ui->stackedInner->setCurrentWidget(ui->front_side);
}
