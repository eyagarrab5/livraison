#include "client.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QObject>

client::client() {
    nom = "";
    prenom = "";
    adresse = "";
    telephone = "";
    email = "";
}

client::client(QString nom, QString prenom, QString adresse, QString telephone, QString email) {
    this->nom = nom;
    this->prenom = prenom;
    this->adresse = adresse;
    this->telephone = telephone;
    this->email = email;
}

int client::get_id() { return id; }
void client::set_nom(QString val) { nom = val; }
QString client::get_nom() { return nom; }
void client::set_prenom(QString val) { prenom = val; }
QString client::get_prenom() { return prenom; }
void client::set_adresse(QString val) { adresse = val; }
QString client::get_adresse() { return adresse; }
void client::set_telephone(QString val) { telephone = val; }
QString client::get_telephone() { return telephone; }
void client::set_email(QString val) { email = val; }
QString client::get_email() { return email; }

bool client::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (NOM, PRENOM, ADRESSE, TELEPHONE, EMAIL) "
                  "VALUES (:nom, :prenom, :adresse, :telephone, :email)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    return query.exec();
}

bool client::modifier(int id, QString nom, QString prenom, QString adresse, QString telephone, QString email) {
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM = ?, PRENOM = ?, ADRESSE = ?, TELEPHONE = ?, EMAIL = ? WHERE ID_CLIENT = ?");
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(adresse);
    query.addBindValue(telephone);
    query.addBindValue(email);
    query.addBindValue(id);
    return query.exec();
}

bool client::supprimer(int idd) {
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id");
    query.bindValue(":id", idd);
    return query.exec();
}

QSqlQueryModel* client::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    return model;
}

QSqlQueryModel* client::trier_id() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY ID_CLIENT ASC");
    return model;
}

QSqlQueryModel* client::trier_nom() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY NOM ASC");
    return model;
}

QSqlQueryModel* client::trier_prenom() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY PRENOM ASC");
    return model;
}
