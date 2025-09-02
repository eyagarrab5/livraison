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
#include "setroles.h"

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
    QString nom = ui->nom_client->text().trimmed();
    QString prenom = ui->prenom_client->text().trimmed();
    QString adresse = ui->adresse_client->text().trimmed();
    QString telephone = ui->num_tel_client->text().trimmed();
    QString email = ui->email_client->text().trimmed();

    // 🔍 Nom et prénom
    if (nom.length() < 3 || prenom.length() < 3) {
        QMessageBox::warning(this, "Nom/Prénom invalide", "Le nom et le prénom doivent contenir au moins 3 caractères.");
        return;
    }

    // 🔍 Adresse
    if (adresse.length() < 5) {
        QMessageBox::warning(this, "Adresse invalide", "L'adresse doit contenir au moins 5 caractères.");
        return;
    }

    // 🔍 Téléphone : format + unicité
    if (!(telephone.startsWith("+216") && telephone.length() == 12) &&
        !(telephone.startsWith("+33") && telephone.length() == 13) &&
        !(telephone.startsWith("+49") && telephone.length() == 13)) {
        QMessageBox::warning(this, "Téléphone invalide", "Le numéro doit commencer par +216 (8 chiffres), +33 (9 chiffres), ou +49 (9 chiffres).");
        return;
    }

    QSqlQuery checkTel;
    checkTel.prepare("SELECT COUNT(*) FROM CLIENT WHERE TELEPHONE = :tel");
    checkTel.bindValue(":tel", telephone);
    if (checkTel.exec() && checkTel.next() && checkTel.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Téléphone existant", "Ce numéro est déjà utilisé par un autre client.");
        return;
    }

    // 🔍 Email : non vide + unicité
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Email manquant", "Veuillez saisir une adresse email.");
        return;
    }

    QSqlQuery checkEmail;
    checkEmail.prepare("SELECT COUNT(*) FROM CLIENT WHERE EMAIL = :email");
    checkEmail.bindValue(":email", email);
    if (checkEmail.exec() && checkEmail.next() && checkEmail.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Email existant", "Cette adresse email est déjà utilisée par un autre client.");
        return;
    }

    client cl(nom, prenom, adresse, telephone, email);
    bool test = cl.ajouter();

    if (test)
    {
        QMessageBox::information(this, "Ajout réussi", "Client ajouté avec succès.");

        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->adresse_client->clear();
        ui->num_tel_client->clear();
        ui->email_client->clear();

        ui->table_clients->setModel(c.afficher());
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "L'ajout n'a pas été effectué. Vérifiez les données ou la connexion à la base.");
    }
}
void MainWindow::on_modifier_4_clicked()
{
    int id = ui->id_client_mod->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID client valide.");
        return;
    }

    QString nom = ui->nom_client->text().trimmed();
    QString prenom = ui->prenom_client->text().trimmed();
    QString adresse = ui->adresse_client->text().trimmed();
    QString telephone = ui->num_tel_client->text().trimmed();
    QString email = ui->email_client->text().trimmed();

    if (nom.length() < 3 || prenom.length() < 3) {
        QMessageBox::warning(this, "Nom/Prénom invalide", "Le nom et le prénom doivent contenir au moins 3 caractères.");
        return;
    }

    if (adresse.length() < 5) {
        QMessageBox::warning(this, "Adresse invalide", "L'adresse doit contenir au moins 5 caractères.");
        return;
    }

    if (!(telephone.startsWith("+216") && telephone.length() == 12) &&
        !(telephone.startsWith("+33") && telephone.length() == 13) &&
        !(telephone.startsWith("+49") && telephone.length() == 13)) {
        QMessageBox::warning(this, "Téléphone invalide", "Le numéro doit commencer par +216 (8 chiffres), +33 (9 chiffres), ou +49 (9 chiffres).");
        return;
    }

    QSqlQuery checkTel;
    checkTel.prepare("SELECT COUNT(*) FROM CLIENT WHERE TELEPHONE = :tel AND ID_CLIENT != :id");
    checkTel.bindValue(":tel", telephone);
    checkTel.bindValue(":id", id);
    if (checkTel.exec() && checkTel.next() && checkTel.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Téléphone existant", "Ce numéro est déjà utilisé par un autre client.");
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Email manquant", "Veuillez saisir une adresse email.");
        return;
    }

    QSqlQuery checkEmail;
    checkEmail.prepare("SELECT COUNT(*) FROM CLIENT WHERE EMAIL = :email AND ID_CLIENT != :id");
    checkEmail.bindValue(":email", email);
    checkEmail.bindValue(":id", id);
    if (checkEmail.exec() && checkEmail.next() && checkEmail.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Email existant", "Cette adresse email est déjà utilisée par un autre client.");
        return;
    }

    bool test = c.modifier(id, nom, prenom, adresse, telephone, email);

    if (test)
    {
        QMessageBox::information(this, "Modification réussie", "Client modifié avec succès.");

        ui->id_client_mod->clear();
        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->adresse_client->clear();
        ui->num_tel_client->clear();
        ui->email_client->clear();

        ui->table_clients->setModel(c.afficher());

        QSystemTrayIcon *notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
        notifyIcon->show();
        notifyIcon->showMessage("Modification effectuée", "Client modifié", QSystemTrayIcon::Information, 15000);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "La modification n'a pas été effectuée. Vérifiez les données ou la connexion à la base.");
    }
}


void MainWindow::on_id_client_mod_textChanged(const QString &arg1)
{
    int id = arg1.toInt();
    if (id > 0)
    {
        chargerDonneesClient(id);
    }
}

void MainWindow::chargerDonneesClient(int id)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, ADRESSE, TELEPHONE, EMAIL "
                  "FROM CLIENT WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        // Remplir automatiquement tous les champs
        ui->nom_client->setText(query.value(0).toString());
        ui->prenom_client->setText(query.value(1).toString());
        ui->adresse_client->setText(query.value(2).toString());
        ui->num_tel_client->setText(query.value(3).toString());
        ui->email_client->setText(query.value(4).toString());
    }
    else
    {
        // Vider les champs si l'ID n'existe pas
        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->adresse_client->clear();
        ui->num_tel_client->clear();
        ui->email_client->clear();
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
    QString nom = ui->nomcom->text().trimmed();
    QString reference = ui->reference->text().trimmed();
    QDate dateObj = ui->dateEdit->date();
    QString date = dateObj.toString("yyyy-MM-dd");
    QString adresse = ui->adresse_liv->text().trimmed();
    QString statut = ui->statut->currentText().trimmed().simplified();

    // 🔍 Nom
    if (nom.length() < 3) {
        QMessageBox::warning(this, "Nom invalide", "Le nom de commande doit contenir au moins 3 caractères.");
        return;
    }

    // 🔍 Référence
    if (reference.isEmpty()) {
        QMessageBox::warning(this, "Référence manquante", "Veuillez saisir une référence.");
        return;
    }

    QSqlQuery checkRef;
    checkRef.prepare("SELECT COUNT(*) FROM COMMANDE WHERE REFERENCE = :ref");
    checkRef.bindValue(":ref", reference);
    if (checkRef.exec() && checkRef.next() && checkRef.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Référence existante", "Cette référence est déjà utilisée.");
        return;
    }

    // 🔍 Date
    if (dateObj < QDate::currentDate()) {
        QMessageBox::warning(this, "Date invalide", "La date de commande doit être aujourd'hui ou dans le futur.");
        return;
    }

    // 🔍 Adresse
    if (adresse.length() < 5) {
        QMessageBox::warning(this, "Adresse invalide", "L'adresse de livraison doit contenir au moins 5 caractères.");
        return;
    }

    // 🔍 Statut
    QStringList statutValide = {"En attente", "En cours", "Livree", "Annulee"};
    if (!statutValide.contains(statut)) {
        QMessageBox::warning(this, "Statut invalide", "Le statut sélectionné n'est pas autorisé.");
        return;
    }

    commande cm(nom, reference, date, adresse, statut);
    bool test = cm.ajouter();

    if (test)
    {
        QMessageBox::information(this, "Ajout réussi", "Commande ajoutée avec succès.");

        ui->nomcom->clear();
        ui->reference->clear();
        ui->adresse_liv->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->statut->setCurrentIndex(0);
        ui->table_commandes->setModel(cmd.afficher());

        QSystemTrayIcon *notifyIcon = new QSystemTrayIcon(this);
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
        notifyIcon->show();
        notifyIcon->showMessage("Ajout effectué", "Commande ajoutée", QSystemTrayIcon::Information, 15000);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "L'ajout n'a pas été effectué. Vérifiez les données ou la connexion à la base.");
    }
}
void MainWindow::on_modifier_6_clicked()
{
    int id = ui->id_comm_mod->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID de commande valide.");
        return;
    }

    QString nom = ui->nomcom->text().trimmed();
    QString reference = ui->reference->text().trimmed();
    QDate dateObj = ui->dateEdit->date();
    QString date = dateObj.toString("yyyy-MM-dd");
    QString adresse = ui->adresse_liv->text().trimmed();
    QString statut = ui->statut->currentText().trimmed().simplified();

    if (nom.length() < 3) {
        QMessageBox::warning(this, "Nom invalide", "Le nom de commande doit contenir au moins 3 caractères.");
        return;
    }

    if (reference.isEmpty()) {
        QMessageBox::warning(this, "Référence manquante", "Veuillez saisir une référence.");
        return;
    }

    QSqlQuery checkRef;
    checkRef.prepare("SELECT COUNT(*) FROM COMMANDE WHERE REFERENCE = :ref AND ID_COMMANDE != :id");
    checkRef.bindValue(":ref", reference);
    checkRef.bindValue(":id", id);
    if (checkRef.exec() && checkRef.next() && checkRef.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Référence existante", "Cette référence est déjà utilisée par une autre commande.");
        return;
    }

    if (dateObj < QDate::currentDate()) {
        QMessageBox::warning(this, "Date invalide", "La date de commande doit être aujourd'hui ou dans le futur.");
        return;
    }

    if (adresse.length() < 5) {
        QMessageBox::warning(this, "Adresse invalide", "L'adresse de livraison doit contenir au moins 5 caractères.");
        return;
    }

    QStringList statutValide = {"En attente", "En cours", "Livree", "Annulee"};
    if (!statutValide.contains(statut)) {
        QMessageBox::warning(this, "Statut invalide", "Le statut sélectionné n'est pas autorisé.");
        return;
    }

    bool test = cmd.modifier(id, nom, reference, date, adresse, statut);

    if (test)
    {
        QMessageBox::information(this, "Modification réussie", "Commande modifiée avec succès.");

        ui->id_comm_mod->clear();
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
        QMessageBox::critical(this, "Erreur", "La modification n'a pas été effectuée. Vérifiez les données ou la connexion à la base.");
    }
}

void MainWindow::on_id_comm_mod_textChanged(const QString &arg1)
{
    int id = arg1.toInt();
    if (id > 0)
    {
        chargerDonneesCommande(id);
    }
}

void MainWindow::chargerDonneesCommande(int id)
{
    QSqlQuery query;
    query.prepare("SELECT NOM_COMMANDE, REFERENCE, DATE_COMMANDE, ADRESSE_LIVRAISON, STATUT "
                  "FROM COMMANDE WHERE ID_COMMANDE = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        // Remplir automatiquement tous les champs
        ui->nomcom->setText(query.value(0).toString());
        ui->reference->setText(query.value(1).toString());

        QDate date = query.value(2).toDate();
        if (date.isValid())
            ui->dateEdit->setDate(date);

        ui->adresse_liv->setText(query.value(3).toString());

        // Trouver l'index du statut dans la combobox
        QString statut = query.value(4).toString();
        int index = ui->statut->findText(statut);
        if (index != -1)
            ui->statut->setCurrentIndex(index);
    }
    else
    {
        // Vider les champs si l'ID n'existe pas
        ui->nomcom->clear();
        ui->reference->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->adresse_liv->clear();
        ui->statut->setCurrentIndex(0);
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

void MainWindow::on_supprimer_13_clicked()
{
    QMap<QString, int> statutCount;

    // Compter les commandes par statut
    QSqlQuery query("SELECT STATUT, COUNT(*) FROM COMMANDE GROUP BY STATUT");
    while (query.next())
    {
        QString statut = query.value(0).toString().toLower();
        int count = query.value(1).toInt();
        statutCount[statut] = count;
    }

    // Récupérer les valeurs spécifiques
    float en_attente = statutCount.value("en attente", 0);
    float en_cours = statutCount.value("en cours", 0);
    float livree = statutCount.value("livrée", 0);
    float annulee = statutCount.value("annulée", 0);

    float total = en_attente + en_cours + livree + annulee;

    QPieSeries *series = new QPieSeries();

    // Ajouter les tranches seulement si le total > 0
    if (total > 0)
    {
        QString a = "En attente " + QString::number((en_attente * 100) / total, 'f', 2) + "%";
        QString b = "En cours " + QString::number((en_cours * 100) / total, 'f', 2) + "%";
        QString c = "Livrée " + QString::number((livree * 100) / total, 'f', 2) + "%";
        QString d = "Annulée " + QString::number((annulee * 100) / total, 'f', 2) + "%";

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
    }
    else
    {
        // Cas où il n'y a aucune commande
        series->append("Aucune commande", 1);
        series->slices().first()->setLabelVisible();
        series->slices().first()->setLabel("Aucune donnée disponible");
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
void MainWindow::setRole(QString role)
{
    // Gestion des commandes
    ui->ajouter_3->setVisible(role == "admin" || role == "logistique");
    ui->modifier_6->setVisible(role == "admin" || role == "logistique");
    ui->supprimer_15->setVisible(role == "admin" || role == "logistique");

    // Affichage des commandes
    ui->commandes_du_jour->setVisible(true); // tous les rôles
    ui->toutes_les_commandes->setVisible(true); // tous les rôles
    ui->table_commandes->setVisible(true); // tous les rôles

    // Tri commandes
    ui->supprimer_9->setVisible(true);  // trie par ID
    ui->supprimer_10->setVisible(true); // trie par nom
    ui->supprimer_11->setVisible(true); // trie par référence

    // Recherche commandes
    ui->lineEdit_2->setVisible(true); // champ de recherche

    // Statistiques commandes
    ui->supprimer_13->setVisible(true); // bouton stats

    // PDF commandes
    ui->supprimer_12->setVisible(true); // bouton PDF

    // Gestion des clients
    ui->ajouter_2->setVisible(role == "admin");
    ui->modifier_4->setVisible(role == "admin");
    ui->supprimer_2->setVisible(role == "admin");

    // Affichage des clients
    ui->table_clients->setVisible(true); // tous les rôles

    // Tri, recherche, stats clients
    ui->lineEdit->setVisible(true);
    ui->supprimer_3->setVisible(true);
    ui->supprimer_4->setVisible(true);
    ui->supprimer_5->setVisible(true);

    ui->supprimer_7->setVisible(true);
    ui->supprimer_6->setVisible(true);
}

void MainWindow::on_btn_retour_roles_clicked()
{
    SetRoles *rolesPage = new SetRoles();
    rolesPage->show();
    this->close(); // Ferme la fenêtre principale
}


