#include "fcback.h"
#include "ui_fcback.h"
#include "server.h"

FCBack::FCBack(QString name, QString text, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCBack)
{
    ui->setupUi(this);
    groupID = name;
    edit_back(text, index);
    ui->back_widget->setCurrentIndex(0);
}

FCBack::~FCBack()
{
    delete ui;
}

void FCBack::edit_back(QString text, int index){
    // Initializes both label, and text box
    ui->back_text->setPlainText(text);
    ui->back_label->setText(text);
    server::send(server::FLASHCARD_SET_BACK + groupID + " " + QString::number(index) + " " + text);

    /*
    if(display_edit){        // sets current index of back widget
        display_edit_back();
    }
    else{
        display_back();
    }
    */
}

// ** PROBABLY NOT NEEDED **
// Displays back label
void FCBack::display_back()
{
    ui->back_label->setText(ui->back_text->toPlainText());
    ui->back_widget->setCurrentIndex(0);
}

// Displays the editing side of the back card
void FCBack::display_edit_back()
{
    QString text = ui->back_label->text();
    ui->back_text->setPlainText(text);
    ui->back_widget->setCurrentIndex(1);
}
// **

void FCBack::on_edit_card_btn_clicked()
{
    ui->back_widget->setCurrentIndex(1);
    QString text = ui->back_label->text();
    ui->back_text->setPlainText(text);
}

void FCBack::on_set_back_btn_clicked()
{
    QString text = ui->back_text->toPlainText();
    edit_back(text, 0);  // index needs to be passed in here
    ui->back_widget->setCurrentIndex(0);
}
