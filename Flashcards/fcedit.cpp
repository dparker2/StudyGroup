#include "fcedit.h"
#include "ui_fcedit.h"

#include "fcside.h"
#include "server.h"

FCEdit::FCEdit(QString group_name, int index, QString label, FCSide *side, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCEdit)
{
    ui->setupUi(this);
    ui->label->setText(label);

    my_group = group_name;
    my_index = index;
    my_side = side;
}

FCEdit::~FCEdit()
{
    delete ui;
}

void FCEdit::set_side_text()
{
    QString text = ui->textedit->toPlainText();
    my_side->set_text(text);
    server::send(my_code + my_group + " " + QString::number(my_index) + " " + text);
}

void FCEdit::get_side_text()
{
    ui->textedit->setPlainText(my_side->get_text());
}
