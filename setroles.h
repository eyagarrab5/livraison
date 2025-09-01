#ifndef SETROLES_H
#define SETROLES_H

#include <QDialog>
namespace Ui {
class SetRoles;
}

class SetRoles : public QDialog{
    Q_OBJECT

public:
    explicit SetRoles(QWidget *parent = nullptr);
    ~SetRoles();

private slots:
    void on_btn_admin_clicked();
    void on_btn_logistique_clicked();
    void on_btn_livreur_clicked();

private:
    Ui::SetRoles *ui;
};

#endif // SETROLES_H
