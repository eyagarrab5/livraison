#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<QSystemTrayIcon>
#include <QRegExp>
#include "connection.h"
#include <QFileDialog>
#include <QPainter>
#include <QDate>
#include <QPdfWriter>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QPieSlice>
#include <QPieSeries>
#include <QtCharts>
#include <QApplication>
#include <QIntValidator>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QtCore>
#include <QIcon>
#include <QtTextToSpeech>
#include <QSystemTrayIcon>
#include <QUrl>
#include <QPixmap>
#include "qrcode.h"

using namespace qrcodegen;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->table_clients->setModel(c.afficher());
    ui->table_commandes->setModel(cmd.afficher());
    ui->id_comm_mod->setVisible(true);
    /////////////////////////// cs ////////////////////////


}


MainWindow::~MainWindow()
{
    delete ui;
}
//////////////////////////////////////////////////// EMPLOYEE ///////////////////////////////////////////////////////////

//ajout employe
void MainWindow::on_ajouter_2_clicked()
{
    QString nom = ui->nom_client->text();
    QString prenom = ui->prenom_client->text();
    QString adresse = ui->adresse_client->text();
    QString telephone = ui->num_tel_client->text();
    QString email = ui->email_client->text();

    client cl(nom, prenom, adresse, telephone, email);
    bool test = cl.ajouter();

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                                 QObject::tr("Client ajouté avec succès."), QMessageBox::Cancel);

        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->adresse_client->clear();
        ui->num_tel_client->clear();
        ui->email_client->clear();

        ui->table_clients->setModel(c.afficher()); // Assure-toi que le nom est bien table_client
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("Ajout non effectué."), QMessageBox::Cancel);
    }
}

// modifier employe
void MainWindow::on_modifier_4_clicked()
{
    int id = ui->id_client_mod->text().toUInt();
    QString nom = ui->nom_client->text();
    QString prenom = ui->prenom_client->text();
    QString adresse = ui->adresse_client->text();
    QString telephone = ui->num_tel_client->text();
    QString email = ui->email_client->text();

    bool test = c.modifier(id, nom, prenom, adresse, telephone, email);

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                                 QObject::tr("Le client a été modifié avec succès."), QMessageBox::Cancel);

        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->adresse_client->clear();
        ui->num_tel_client->clear();
        ui->email_client->clear();

        ui->table_clients->setModel(c.afficher());
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("La modification n'a pas été effectuée."), QMessageBox::Cancel);
    }
}

//supprimer employe
void MainWindow::on_supprimer_2_clicked()
{
    int id = ui->id_client_2->text().toUInt();
    bool test = c.supprimer(id);

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                 QObject::tr("Le client a été supprimé."), QMessageBox::Cancel);

        ui->id_client_mod->clear();
        ui->table_clients->setModel(c.afficher());
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("La suppression n'a pas été effectuée."), QMessageBox::Cancel);
    }
}



// trie id client
void MainWindow::on_supprimer_3_clicked()
{
    ui->table_clients->setModel(c.trier_id());
}

// trie nom client
void MainWindow::on_supprimer_4_clicked()
{
    ui->table_clients->setModel(c.trier_nom());
}

// trie prénom client
void MainWindow::on_supprimer_5_clicked()
{
    ui->table_clients->setModel(c.trier_prenom());
}

// pdf employe
void MainWindow::on_supprimer_6_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choisir un dossier"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) return;

    QPdfWriter pdf(dir + "/ListeClients.pdf");
    QPainter painter(&pdf);
    int i = 4000;

    painter.drawPixmap(QRect(100, 100, 2500, 2500), QPixmap("C:/")); // image facultative
    painter.setPen(Qt::red);
    painter.setFont(QFont("Times New Roman", 25));
    painter.drawText(3000, 1400, "Liste des Clients");

    painter.setPen(Qt::black);
    painter.setFont(QFont("Times New Roman", 15));
    painter.drawRect(100, 100, 9400, 2500); // cadre principal
    painter.drawRect(100, 3000, 9400, 500); // en-tête

    painter.setFont(QFont("Times New Roman", 9));
    painter.drawText(300, 3300, "ID");
    painter.drawText(1500, 3300, "Nom");
    painter.drawText(3000, 3300, "Prénom");
    painter.drawText(4500, 3300, "Adresse");
    painter.drawText(6000, 3300, "Téléphone");
    painter.drawText(7500, 3300, "Email");

    painter.drawRect(100, 3000, 9400, 10700); // tableau

    QSqlQuery query;
    query.prepare("SELECT * FROM CLIENT");
    query.exec();

    while (query.next())
    {
        painter.drawText(300, i, query.value(0).toString());
        painter.drawText(1500, i, query.value(1).toString());
        painter.drawText(3000, i, query.value(2).toString());
        painter.drawText(4500, i, query.value(3).toString());
        painter.drawText(6000, i, query.value(4).toString());
        painter.drawText(7500, i, query.value(5).toString());
        i += 500;
    }

    painter.end();

    int reponse = QMessageBox::question(this, "Générer PDF",
                                        "PDF enregistré. Voulez-vous l’ouvrir ?",
                                        QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir + "/ListeClients.pdf"));
    }
}

void MainWindow::on_supprimer_7_clicked()
{
    QMap<QString, int> indicatifCount;
    QSqlQuery query("SELECT TELEPHONE FROM CLIENT");

    while (query.next())
    {
        QString tel = query.value(0).toString();

        if (tel.startsWith("+216"))
            indicatifCount["Tunisie"]++;
        else if (tel.startsWith("+33"))
            indicatifCount["France"]++;
        else if (tel.startsWith("+49"))
            indicatifCount["Allemagne"]++;
        else if (tel.startsWith("+1"))
            indicatifCount["États-Unis / Canada"]++;
        else if (tel.startsWith("+44"))
            indicatifCount["Royaume-Uni"]++;
        else
            indicatifCount["Autres"]++;
    }

    int total = 0;
    for (auto count : indicatifCount.values())
        total += count;

    QPieSeries *series = new QPieSeries();
    for (auto it = indicatifCount.begin(); it != indicatifCount.end(); ++it)
    {
        QString label = it.key() + " " + QString::number((it.value() * 100.0) / total, 'f', 2) + "%";
        series->append(label, it.value());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des clients par indicatif téléphonique");
    chart->legend()->show();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(1000, 500);
    chartView->show();
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery();

    query->prepare("SELECT * FROM CLIENT WHERE ID_CLIENT LIKE :val OR NOM LIKE :val OR PRENOM LIKE :val OR TELEPHONE LIKE :val OR EMAIL LIKE :val");
    query->bindValue(":val", arg1 + "%");
    query->exec();

    if (query->next())
    {
        model->setQuery(*query);
        ui->table_clients->setModel(model);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Recherche"),
                              QObject::tr("Aucun client trouvé.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->lineEdit->clear();
    }
}


//////////////////////////////////////////////////// ENTREPRISE ///////////////////////////////////////////////////////////
void MainWindow::on_ajouter_3_clicked()
{
    QString nom = ui->nomcom->text();
    QString reference = ui->reference->text();
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString adresse = ui->adresse_liv->text();
    QString statut = ui->statut->currentText();

    commande cm(nom, reference, date, adresse, statut);
    bool test = cm.ajouter();

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                                 QObject::tr("Commande ajoutée avec succès."), QMessageBox::Cancel);

        ui->nomcom->clear();
        ui->reference->clear();
        ui->adresse_liv->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->statut->setCurrentIndex(0);

        ui->table_commandes->setModel(cmd.afficher());

        QSystemTrayIcon *notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg")); // Assure-toi que ce chemin est valide
        notifyIcon->show();
        notifyIcon->showMessage("Ajout effectué", "Commande ajoutée", QSystemTrayIcon::Information, 15000);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("L'ajout de la commande n'a pas été effectué."), QMessageBox::Cancel);
    }
}


// modifier entreprise
void MainWindow::on_modifier_6_clicked()
{
    int id = ui->id_comm_mod->text().toUInt();
    QString nom = ui->nomcom->text();
    QString reference = ui->reference->text();
    QString date = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString adresse = ui->adresse_liv->text();
    QString statut = ui->statut->currentText();

    bool test = cmd.modifier(id, nom, reference, date, adresse, statut);

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                                 QObject::tr("La commande a été modifiée."), QMessageBox::Cancel);

        ui->nomcom->clear();
        ui->reference->clear();
        ui->adresse_liv->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->statut->setCurrentIndex(0);

        ui->table_commandes->setModel(cmd.afficher());

        QSystemTrayIcon *notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
        notifyIcon->show();
        notifyIcon->showMessage("Modification effectuée", "Commande modifiée", QSystemTrayIcon::Information, 15000);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("La modification n'a pas été effectuée."), QMessageBox::Cancel);
    }
}

// supprimer entreprise
void MainWindow::on_supprimer_15_clicked()
{
    QString idText = ui->id_commande_3->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "ID manquant", "Veuillez saisir l'ID de la commande à supprimer.");
        return;
    }

    bool ok;
    int id = idText.toInt(&ok);

    if (!ok || id <= 0) {
        QMessageBox::warning(this, "ID invalide", "L'ID saisi n'est pas un entier valide.");
        return;
    }

    bool test = cmd.supprimer(id);

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                 QObject::tr("La commande a été supprimée."), QMessageBox::Cancel);

        ui->id_comm_mod->clear();
        ui->table_commandes->setModel(cmd.afficher());

        QSystemTrayIcon *notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
        notifyIcon->show();
        notifyIcon->showMessage("Suppression effectuée", "Commande supprimée", QSystemTrayIcon::Information, 15000);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec"),
                              QObject::tr("La suppression n'a pas été effectuée."), QMessageBox::Cancel);
    }
}

// trie id commande
void MainWindow::on_supprimer_9_clicked()
{
    ui->table_commandes->setModel(cmd.trie_id());
}

// trie nom commande
void MainWindow::on_supprimer_10_clicked()
{
    ui->table_commandes->setModel(cmd.trie_nom_commande());
}

// trie référence commande
void MainWindow::on_supprimer_11_clicked()
{
    ui->table_commandes->setModel(cmd.trie_reference());
}
void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery();

    query->prepare("SELECT * FROM COMMANDE WHERE ID_COMMANDE LIKE :val OR NOM_COMMANDE LIKE :val OR REFERENCE LIKE :val OR ADRESSE_LIVRAISON LIKE :val OR STATUT LIKE :val");
    query->bindValue(":val", arg1 + "%");
    query->exec();

    if (query->next())
    {
        model->setQuery(*query);
        ui->table_commandes->setModel(model);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("SEARCH"),
                              QObject::tr("NO MATCH FOUND !!\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->lineEdit_2->clear();
    }
}

// stat entreprise
void MainWindow::on_supprimer_13_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM COMMANDE WHERE STATUT = 'en attente'");
    float en_attente = model->rowCount();

    model->setQuery("SELECT * FROM COMMANDE WHERE STATUT = 'en cours'");
    float en_cours = model->rowCount();

    model->setQuery("SELECT * FROM COMMANDE WHERE STATUT = 'livrée'");
    float livree = model->rowCount();

    model->setQuery("SELECT * FROM COMMANDE WHERE STATUT = 'annulée'");
    float annulee = model->rowCount();

    float total = en_attente + en_cours + livree + annulee;

    QString a = "En attente " + QString::number((en_attente * 100) / total, 'f', 2) + "%";
    QString b = "En cours " + QString::number((en_cours * 100) / total, 'f', 2) + "%";
    QString c = "Livrée " + QString::number((livree * 100) / total, 'f', 2) + "%";
    QString d = "Annulée " + QString::number((annulee * 100) / total, 'f', 2) + "%";

    QPieSeries *series = new QPieSeries();
    series->append(a, en_attente);
    series->append(b, en_cours);
    series->append(c, livree);
    series->append(d, annulee);

    for (int i = 0; i < series->slices().size(); ++i)
    {
        QPieSlice *slice = series->slices().at(i);
        if (slice->value() > 0)
            slice->setLabelVisible();
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des commandes par statut — Total : " + QString::number(total));
    chart->legend()->show();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(1000, 500);
    chartView->show();
}
void MainWindow::on_supprimer_12_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choisir un dossier"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) return;

    QPdfWriter pdf(dir + "/ListeCommandes.pdf");
    QPainter painter(&pdf);
    int i = 4000;

    painter.drawPixmap(QRect(100, 100, 2500, 2500), QPixmap("C:/")); // image facultative
    painter.setPen(Qt::red);
    painter.setFont(QFont("Times New Roman", 25));
    painter.drawText(3000, 1400, "Liste des commandes");

    painter.setPen(Qt::black);
    painter.setFont(QFont("Times New Roman", 15));
    painter.drawRect(100, 100, 9400, 2500);
    painter.drawRect(100, 3000, 9400, 500);

    painter.setFont(QFont("Times New Roman", 9));
    painter.drawText(300, 3300, "ID");
    painter.drawText(1000, 3300, "Nom");
    painter.drawText(2500, 3300, "Référence");
    painter.drawText(4000, 3300, "Date");
    painter.drawText(5500, 3300, "Adresse");
    painter.drawText(7000, 3300, "Statut");

    painter.drawRect(100, 3000, 9400, 10700);

    QSqlQuery query("SELECT * FROM COMMANDE");
    while (query.next())
    {
        painter.drawText(300, i, query.value(0).toString());
        painter.drawText(1000, i, query.value(5).toString()); // NOM_COMMANDE
        painter.drawText(2500, i, query.value(1).toString()); // REFERENCE
        painter.drawText(4000, i, query.value(2).toString()); // DATE_COMMANDE
        painter.drawText(5500, i, query.value(3).toString()); // ADRESSE_LIVRAISON
        painter.drawText(7000, i, query.value(4).toString()); // STATUT
        i += 500;
    }

    painter.end();

    int reponse = QMessageBox::question(this, "Générer PDF",
                                        "PDF enregistré. Voulez-vous l’ouvrir ?",
                                        QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir + "/ListeCommandes.pdf"));
    }
}





// Voir les commandes du jour
void MainWindow::on_commandes_du_jour_clicked()
{
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery();

    query->prepare("SELECT * FROM COMMANDE WHERE DATE_COMMANDE = TO_DATE(:today, 'YYYY-MM-DD')");
    query->bindValue(":today", today);
    query->exec();
    model->setQuery(*query);

    ui->table_commandes->setModel(model);
}

// Voir toutes les commandes
void MainWindow::on_toutes_les_commandes_clicked()
{
    ui->table_commandes->setModel(cmd.afficher());
}

