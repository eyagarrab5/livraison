#include "setroles.h"
#include "ui_setroles.h"
#include "mainwindow.h"

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

// Bouton Administrateur
void SetRoles::on_btn_admin_clicked()
{
    MainWindow *mw = new MainWindow();
    mw->setRole("admin");
    mw->show();
    this->close();
}

// Bouton Gestionnaire logistique
void SetRoles::on_btn_logistique_clicked()
{
    MainWindow *mw = new MainWindow();
    mw->setRole("logistique");
    mw->show();
    this->close();
}

// Bouton Livreur
void SetRoles::on_btn_livreur_clicked()
{
    MainWindow *mw = new MainWindow();
    mw->setRole("livreur");
    mw->show();
    this->close();
}
