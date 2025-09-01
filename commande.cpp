#include "commande.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QObject>
#include <QDebug>

commande::commande() {
    nom_commande = "";
    reference = "";
    date_commande = "";
    adresse_livraison = "";
    statut = "";
}

commande::commande(QString nom_commande, QString reference, QString date_commande, QString adresse_livraison, QString statut) {
    this->nom_commande = nom_commande;
    this->reference = reference;
    this->date_commande = date_commande;
    this->adresse_livraison = adresse_livraison;
    this->statut = statut;
}

int commande::get_id() { return id; }
void commande::Set_nom_commande(QString val) { nom_commande = val; }
QString commande::get_nom_commande() { return nom_commande; }
void commande::Set_reference(QString val) { reference = val; }
QString commande::get_reference() { return reference; }
void commande::Set_date_commande(QString val) { date_commande = val; }
QString commande::get_date_commande() { return date_commande; }
void commande::Set_adresse_livraison(QString val) { adresse_livraison = val; }
QString commande::get_adresse_livraison() { return adresse_livraison; }
void commande::Set_statut(QString val) { statut = val; }
QString commande::get_statut() { return statut; }

bool commande::ajouter()
{
    statut = statut.trimmed().simplified();

    QStringList statutValide = {"En attente", "En cours", "Livree", "Annulee"};
    if (!statutValide.contains(statut)) {
        qDebug() << "Statut invalide:" << statut;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO COMMANDE (NOM_COMMANDE, REFERENCE, DATE_COMMANDE, ADRESSE_LIVRAISON, STATUT) "
                  "VALUES (:nom, :ref, TO_DATE(:date, 'YYYY-MM-DD'), :adresse, :statut)");

    query.bindValue(":nom", nom_commande);
    query.bindValue(":ref", reference);
    query.bindValue(":date", date_commande);
    query.bindValue(":adresse", adresse_livraison);
    query.bindValue(":statut", statut);

    return query.exec();
}


bool commande::modifier(int id, QString nom, QString reference, QString date, QString adresse, QString statut)
{
    QSqlQuery query;
    query.prepare("UPDATE COMMANDE SET "
                  "NOM_COMMANDE = :nom, "
                  "REFERENCE = :reference, "
                  "DATE_COMMANDE = TO_DATE(:date, 'YYYY-MM-DD'), " // Utiliser TO_DATE
                  "ADRESSE_LIVRAISON = :adresse, "
                  "STATUT = :statut "
                  "WHERE ID_COMMANDE = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":reference", reference);
    query.bindValue(":date", date);
    query.bindValue(":adresse", adresse);
    query.bindValue(":statut", statut);
    query.bindValue(":id", id);

    return query.exec();
}

bool commande::supprimer(int idd) {
    QSqlQuery query;
    query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE = :id");
    query.bindValue(":id", idd);
    return query.exec();
}

QSqlQueryModel* commande::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse Livraison"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nom Commande"));
    return model;
}

QSqlQueryModel* commande::trie_id() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE ORDER BY ID_COMMANDE ASC");
    return model;
}

QSqlQueryModel* commande::trie_nom_commande() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE ORDER BY NOM_COMMANDE ASC");
    return model;
}

QSqlQueryModel* commande::trie_reference() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMMANDE ORDER BY REFERENCE ASC");
    return model;
}
