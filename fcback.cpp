#include "fcback.h"
#include "ui_fcback.h"

FCBack::FCBack(QString, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCBack)
{
    ui->setupUi(this);
}

FCBack::~FCBack()
{
    delete ui;
}

void FCBack::edit_back(QString text, bool display_edit){
    // Initializes both label, and text box
    ui->back_text->setPlainText(text);
    ui->back_label->setText(text);

    if(display_edit){        // sets current index of back widget
        display_edit_back();
    }
    else{
        display_back();
    }
}
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

