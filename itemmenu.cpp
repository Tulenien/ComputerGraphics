#include "itemmenu.h"
#include "ui_itemmenu.h"

itemMenu::itemMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemMenu)
{
    ui->setupUi(this);
}

itemMenu::~itemMenu()
{
    delete ui;
}

void itemMenu::setCurrentIndex(int index)
{
    currentIndex = index;
}
