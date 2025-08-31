#include "setroles.h"
#include "ui_setroles.h"

SetRoles::SetRoles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetRoles)
{
    ui->setupUi(this);
}

SetRoles::~SetRoles()
{
    delete ui;
}
