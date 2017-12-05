#include "cardwidget.h"
#include <QInputDialog>
#include <QHBoxLayout>
#include <QDebug>
#include <QDir>

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    ui->next_btn->hide();
    ui->prev_btn->hide();

    current_index = -1;
}
void CardWidget::insertCard(int index, QString front_text){
    if(index < deck.size()) {
        deck.at(index)->setFront(front_text);
    }
    else {
        Flashcard* new_card = new Flashcard(front_text, index);
        qDebug() << "blah";
        deck.insert(index, new_card);
        connect(new_card, SIGNAL(check_set_card(Flashcard*,QString&,int&,int)), this, SLOT(check_set_card(Flashcard*,QString&,int&,int)));
        qDebug() << index << deck.size();
        if(deck.size() > 1)
        {
            ui->prev_btn->show(); // Remove the edited card and show the prev/next buttons
            ui->next_btn->show();
        }
        else {
            qDebug() << "WAT" << deck.size();
            current_index = index; // If first card, set that as the current index and show
            ui->stackedWidget_card_edit->addWidget(new_card);
            ui->stackedWidget_card_edit->setCurrentWidget(new_card);
        }
    }
}
void CardWidget::editCard(int index, QString back_text, QString front_text)
{
    if(front_text != nullptr)
    {
        deck.at(index)->setFront(front_text);
    }
    if(back_text != nullptr)
    {
        deck.at(index)->setBack(back_text);
    }
}
QString CardWidget::get_card_text(){
    //return flashcard->get_card_text();
}

void CardWidget::deleteCard(int index){
    Flashcard* temp = deck[index];
    deck[index] =deck[deck.size()-1];
    deck[deck.size()-1] = temp;
    deck.pop_back();
}

void CardWidget::on_addCardBtn_clicked()
{
    Flashcard* new_card = new Flashcard();
    connect(new_card, SIGNAL(check_set_card(Flashcard*,QString&,int&,int)), this, SLOT(check_set_card(Flashcard*,QString&,int&,int)));
    new_card->emit_init_signal();

    ui->stackedWidget_card_edit->addWidget(new_card);
    ui->stackedWidget_card_edit->setCurrentWidget(new_card); // Put card on front to edit
}

int CardWidget::getDeckSize()
{
    return deck.size();
}

void CardWidget::check_set_card(Flashcard* card, QString& front_text, int& index, int side)
{
    qDebug() << "CHECK_SET_CARD" << endl;
    if(side == 0){
        emit set_card(front_text, index, side); // The index returned is index
        qDebug() << index << deck.size();
        if(deck.indexOf(card) == -1) {
            deck.insert(index, card);
        }
        qDebug() << index << deck.size();
    }
    else{
        // Done editing now. So, delete the "edit" stacked widget if there are more than one
        qDebug() << index << deck.size();
        if(deck.size() > 1)
        {
            qDebug() << "removing";
            if(ui->stackedWidget_card_edit->currentIndex() > 0) {
                ui->stackedWidget_card_edit->removeWidget(card);
            }
            ui->prev_btn->show(); // Remove the edited card and show the prev/next buttons
            ui->next_btn->show();
        }
        else {
            current_index = index; // If first card, set that as the current index
        }
        emit set_card(front_text, index, side);

    }
}

void CardWidget::on_prev_btn_clicked()
{
    ui->stackedWidget_card_edit->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    current_index = (current_index - 1) < 0 ? deck.size() - 1 : current_index - 1; // Update index
    // If current index - 1 is negative, loop back to top. Otherwise, current_index - 1.

    ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Add the widget at the new index
    ui->stackedWidget_card_edit->setCurrentWidget(deck.at(current_index)); // Make sure its the one being displayed

}

void CardWidget::on_next_btn_clicked()
{

    ui->stackedWidget_card_edit->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    current_index = (current_index + 1) % deck.size(); // Update index, mod so it loops back to beginning if too far

    if(quiz){  // not sure if we should do it like this, also do we need button for end quiz?
        current_index = rand() % deck.size();
    }

    ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Add the widget at the new index
    ui->stackedWidget_card_edit->setCurrentWidget(deck.at(current_index)); // Make sure its the one being displayed
}

void CardWidget::quiz()
{
    quiz = true;
    ui->stackedWidget_card_edit->removeWidget(deck[current_index]);  // remove current displayed widget
    ui->stackedWidget_card_edit->addWidget(deck[rand() % deck.size()]);

}

