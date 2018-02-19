#include "fcback.h"
#include "ui_fcback.h"
#include "server.h"

FCBack::FCBack(QString group_name, QString text, int index, bool send_card, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCBack)
{
    ui->setupUi(this);

    groupID = group_name;
    edit_back(text, index, send_card);
    ui->back_widget->setCurrentIndex(0);
    current_index = index;
}

FCBack::~FCBack()
{
    delete ui;
}

void FCBack::edit_back(QString text, int index, bool send_card){
    // Initializes both label, and text box

    qDebug() << "SENDING-> " + server::FLASHCARD_SET_BACK + " " + groupID + " " + QString::number(index) + " " + text;

    if(send_card){
        server::send(server::FLASHCARD_SET_BACK +  groupID + " " + QString::number(index) + " " + text);
    }

    ui->back_text->setPlainText(text);
    ui->back_label->setText(text);

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
void FCBack::on_set_back_btn_clicked()
{
    QString text = ui->back_text->toPlainText();

    edit_back(text, current_index, true);  // index needs to be passed in here
    ui->back_widget->setCurrentIndex(0);
}
void FCBack::set_index(int index){
    current_index = index;
}

