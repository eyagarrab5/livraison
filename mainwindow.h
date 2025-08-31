#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commande.h"
#include "client.h"
#include <QApplication>
#include <QCamera>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_modifier_4_clicked();

    void on_ajouter_2_clicked();


    void on_lineEdit_textChanged(const QString &arg1);

    void on_supprimer_3_clicked();

    void on_supprimer_4_clicked();

    void on_supprimer_5_clicked();


    void on_supprimer_6_clicked();

    void on_supprimer_7_clicked();



    void on_supprimer_2_clicked();

    void on_modifier_6_clicked();

    void on_ajouter_3_clicked();


    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_supprimer_9_clicked();

    void on_supprimer_10_clicked();

    void on_supprimer_11_clicked();

    void on_supprimer_12_clicked();

    void on_supprimer_13_clicked();


    void on_supprimer_15_clicked();


    void on_commandes_du_jour_clicked();

    void on_toutes_les_commandes_clicked();


private:
    Ui::MainWindow *ui;
    commande cmd;
    client c;
};
#endif // MAINWINDOW_H
