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
    ui->table_employe_2->setModel(e.afficher());
    ui->table_employe_3->setModel(en.afficher());
    ui->id_employe_2->setVisible(false);
    populateEntrepriseComboBox();
    /////////////////////////// cs ////////////////////////


}


MainWindow::~MainWindow()
{
    delete ui;
}
//////////////////////////////////////////////////// EMPLOYEE ///////////////////////////////////////////////////////////
/// alimentation combobox ajout employee ac ide
void MainWindow::populateEntrepriseComboBox()
{
    // Créer une instance de QSqlQuery pour exécuter la requête
    QSqlQuery query;
    query.prepare("SELECT id FROM entreprise");

    // Exécuter la requête
    if(query.exec())
    {
        // Vider le QComboBox avant de le remplir
        ui->ide->clear();

        // Parcourir les résultats de la requête
        while(query.next())
        {
            // Récupérer l'id de l'entreprise
            int id = query.value(0).toInt();

            // Ajouter l'id au QComboBox
            ui->ide->addItem(QString::number(id));
        }
    }
    else
    {
        // En cas d'erreur lors de l'exécution de la requête
        qDebug() << "Erreur lors de l'exécution de la requête: " << query.lastError().text();
    }
}
//ajout employe
void MainWindow::on_ajouter_2_clicked()
{
    QString nom =ui->nom_2->text();
    QString prenom =ui->prenom_2->text();
    QString adresse =ui->adresse_2->text();
    QString num =ui->num_tel_2->text();
    QString genre =ui->comboBox->currentText();
    int ideen = ui->ide->currentText().toUInt();
    employee e(nom,prenom,adresse,num,genre,ideen);
    bool test=e.ajouter();
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("ajoute \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->nom_2->clear();
        ui->prenom_2->clear();
        ui->adresse_2->clear();
        ui->num_tel_2->clear();
        ui->table_employe_2->setModel(e.afficher());
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok") , QObject::tr("non effecue"),QMessageBox::Cancel);
}
// modifier employe
void MainWindow::on_modifier_4_clicked()
{
    int id =ui->id_employe_2->text().toUInt();
    QString nom =ui->nom_2->text();
    QString prenom =ui->prenom_2->text();
    QString adresse =ui->adresse_2->text();
    QString num =ui->num_tel_2->text();
    QString genre =ui->comboBox->currentText();
    int ideen = ui->ide->currentText().toUInt();
    bool test=e.modifier(id ,nom,prenom,adresse,num,genre,ideen);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("modifié \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->nom_2->clear();
        ui->prenom_2->clear();
        ui->adresse_2->clear();
        ui->num_tel_2->clear();
        ui->table_employe_2->setModel(e.afficher());

    }
else
    QMessageBox::critical(nullptr,QObject::tr("not ok"),
                          QObject::tr("modification non effectué"),QMessageBox::Cancel);
}
//supprimer employe
void MainWindow::on_supprimer_2_clicked()
{
    int id =ui->id_employes_2->text().toUInt();
    bool test=e.supprimer(id);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Supprimé \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->id_employes_2->clear();
        ui->table_employe_2->setModel(e.afficher());

    }
    else
    QMessageBox::critical(nullptr,QObject::tr("not ok"),
                          QObject::tr("suppression non effectué"),QMessageBox::Cancel);
}
//remplissage employe
void MainWindow::on_table_employe_2_activated(const QModelIndex &index)
{
    QString val=ui->table_employe_2->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("select * from employee  where id='"+val+"' or nom='"+val+"' or prenom='"+val+"' or adresse='"+val+"' or num='"+val+"'");
    if (qry.exec())
    { while (qry.next())
        {
            //update
            //id
            ui->id_employe_2->setText(qry.value(0).toString());
            ui->nom_2->setText(qry.value(1).toString());
            ui->prenom_2->setText(qry.value(2).toString());
            ui->adresse_2->setText(qry.value(3).toString());
            ui->num_tel_2->setText(qry.value(4).toString());
            ui->ide->setCurrentText(qry.value(5).toString());
            //delete
            //id
            ui->id_employes_2->setText(qry.value(0).toString());
        }
    }
}
// trie id employe
void MainWindow::on_supprimer_3_clicked()
{
    ui->table_employe_2->setModel(e.trie_id());
}
// trie nom employe
void MainWindow::on_supprimer_4_clicked()
{
    ui->table_employe_2->setModel(e.trie_nom());
}
// trie prenom employe
void MainWindow::on_supprimer_5_clicked()
{
    ui->table_employe_2->setModel(e.trie_prenom());
}
// pdf employe
void MainWindow::on_supprimer_6_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    qDebug()<<dir;
    QPdfWriter pdf(dir+"/PdfList.pdf");
                           QPainter painter(&pdf);
                          int i = 4000;

                               painter.drawPixmap(QRect(100,100,2500,2500),QPixmap("C:/"));
                               painter.drawText(900,650,"Rh");
                               painter.setPen(Qt::red);
                               painter.setFont(QFont("Time New Roman", 25));
                               painter.drawText(3000,1400,"Liste des employees");
                               painter.setPen(Qt::black);
                               painter.setFont(QFont("Time New Roman", 15));
                               painter.drawRect(100,100,9400,2500); // dimension taa rectangle li fih liste
                               painter.drawRect(100,3000,9400,500);
                               painter.setFont(QFont("Time New Roman", 9));
                               painter.drawText(300,3300,"ID");
                               painter.drawText(2000,3300,"Nom");
                               painter.drawText(4000,3300,"Prenom");
                               painter.drawText(5600,3300,"Adresse");
                               painter.drawText(7000,3300,"Num tel");
                               painter.drawRect(100,3000,9400,10700);
                               //QTextDocument previewDoc;
                               QString pdflist = QDate::currentDate().toString("'data_'MM_dd_yyyy'.txt'");
                               //QTextCursor cursor(&previewDoc);
                               QSqlQuery query;
                               query.prepare("select * from employee");
                               query.exec();
                               while (query.next())
                               {
                                   painter.drawText(300,i,query.value(0).toString());
                                   painter.drawText(2000,i,query.value(1).toString());
                                   painter.drawText(4000,i,query.value(2).toString());
                                   painter.drawText(5600,i,query.value(3).toString());
                                   painter.drawText(7000,i,query.value(4).toString());
                                  i = i +500;
                               }
                               int reponse = QMessageBox::question(this, "Générer PDF", "<PDF Enregistré>...Vous Voulez Affichez Le PDF ?",
                                                                   QMessageBox::Yes|QMessageBox::No);
                                   if (reponse == QMessageBox::Yes)
                                   {
                                       QDesktopServices::openUrl(QUrl::fromLocalFile(dir+"/PdfList.pdf"));

                                       painter.end();
                                   }
                                   else
                                   {
                                        painter.end();
                                   }
}
//stat employe
void MainWindow::on_supprimer_7_clicked()
{
    QSqlQueryModel * model= new QSqlQueryModel();
                                    model->setQuery("select * from employee where genre = 'homme' ");
                                    float e=model->rowCount();
                                    model->setQuery("select * from employee where genre = 'femme' ");
                                    float ee=model->rowCount();
                                    float total=e+ee;
                                    QString a=QString("homme"+QString::number((e*100)/total,'f',2)+"%" );
                                    QString b=QString("femme"+QString::number((ee*100)/total,'f',2)+"%" );
                                    QPieSeries *series = new QPieSeries();
                                    series->append(a,e);
                                    series->append(b,ee);
                            if (e!=0)
                            {QPieSlice *slice = series->slices().at(0);
                             slice->setLabelVisible();
                             slice->setPen(QPen());}
                            if ( ee!=0)
                            {
                                     // Add label, explode and define brush for 2nd slice
                                     QPieSlice *slice1 = series->slices().at(1);
                                     //slice1->setExploded();
                                     slice1->setLabelVisible();
                            }
                                    // Create the chart widget
                                    QChart *chart = new QChart();
                                    // Add data to chart with title and hide legend
                                    chart->addSeries(series);
                                    chart->setTitle("Pourcentage des homme et femme : nombre total des employee: "+ QString::number(total));
                                    chart->legend()->hide();
                                    // Used to display the chart
                                    QChartView *chartView = new QChartView(chart);
                                    chartView->setRenderHint(QPainter::Antialiasing);
                                    chartView->resize(1000,500);
                                    chartView->show();
}
// text to speech employe
void MainWindow::on_supprimer_8_clicked()
{
            QString nom = ui->nom_2->text();
            QString prenom=ui->prenom_2->text();
            QTextToSpeech *s = new QTextToSpeech(this);
            s->setVolume(1.0);
            s->setPitch(1.2);
            s->setRate(0.1);
            s->say(nom+" "+prenom+" est demandé en administration ");
}
// chercher employe
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QSqlQueryModel *model= new QSqlQueryModel();
        QSqlQuery   *query= new QSqlQuery();
        query->prepare("SELECT * FROM employee WHERE id  LIKE'"+arg1+"%' or nom  LIKE'"+arg1+"%' or prenom  LIKE'"+arg1+"%' or num LIKE'"+arg1+"%'");
         query->exec();
         if (query->next()) {
         model->setQuery(*query);
         ui->table_employe_2->setModel(model);
         }
         else {
             QMessageBox::critical(nullptr, QObject::tr("SEARCH"),
                             QObject::tr("NO MATCH FOUND !!\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);
          ui->lineEdit->clear();}
}

//////////////////////////////////////////////////// ENTREPRISE ///////////////////////////////////////////////////////////
// ajouter entreprise
void MainWindow::on_ajouter_3_clicked()
{
    QString nom =ui->nome->text();
    QString adresse =ui->adressee->text();
    QString secteur =ui->secteure->currentText();
    entreprise en(nom,adresse,secteur);
    bool test=en.ajouter();
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("ajoute \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->nome->clear();
        ui->adressee->clear();
        ui->table_employe_3->setModel(en.afficher());
        QSystemTrayIcon * notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
            notifyIcon->show();
            notifyIcon->showMessage("ajout effectue","entreprise ajoute",QSystemTrayIcon::Information,15000);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok") , QObject::tr("non effecue"),QMessageBox::Cancel);
}

// modifier entreprise
void MainWindow::on_modifier_6_clicked()
{
    int id =ui->id_employe_2->text().toUInt();
    QString nom =ui->nome->text();
    QString adresse =ui->adressee->text();
    QString secteur =ui->secteure->currentText();
    bool test=en.modifier(id,nom,adresse,secteur);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("modifié \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->nome->clear();
        ui->adressee->clear();
        ui->table_employe_3->setModel(en.afficher());
        ui->modifier_5->setCurrentIndex(1);
        populateEntrepriseComboBox();
        QSystemTrayIcon * notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
            notifyIcon->show();
            notifyIcon->showMessage("modification effectue","entreprise modifie",QSystemTrayIcon::Information,15000);

    }
else
    QMessageBox::critical(nullptr,QObject::tr("not ok"),
                          QObject::tr("modification non effectué"),QMessageBox::Cancel);
}
// supprimer entreprise
void MainWindow::on_supprimer_15_clicked()
{
    int id =ui->id_employes_3->text().toUInt();
    bool test=en.supprimer(id);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("Supprimé \n"
                                             "click to cancel"), QMessageBox::Cancel);
        ui->id_employes_3->clear();
        ui->table_employe_3->setModel(en.afficher());
        ui->modifier_5->setCurrentIndex(1);
        populateEntrepriseComboBox();
        QSystemTrayIcon * notifyIcon = new QSystemTrayIcon();
        notifyIcon->setIcon(QIcon("C:/images.jpg"));
            notifyIcon->show();
            notifyIcon->showMessage("supression effectue","entreprise supprime",QSystemTrayIcon::Information,15000);

    }
    else
    QMessageBox::critical(nullptr,QObject::tr("not ok"),
                          QObject::tr("suppression non effectué"),QMessageBox::Cancel);
}// remplissage ln entreprise
void MainWindow::on_table_employe_3_activated(const QModelIndex &index)
{
    QString val=ui->table_employe_3->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("select * from entreprise  where id='"+val+"' or nom='"+val+"' or adresse='"+val+"'");
    if (qry.exec())
    { while (qry.next())
        {
            //update
            //id
            ui->id_employe_2->setText(qry.value(0).toString());
            ui->nome->setText(qry.value(1).toString());
            ui->adressee->setText(qry.value(2).toString());
            ui->secteure->setCurrentText(qry.value(3).toString());
            //delete
            //id
            ui->id_employes_3->setText(qry.value(0).toString());
        }
    }
}
// trie nom entreprise
void MainWindow::on_supprimer_10_clicked()
{
    ui->table_employe_3->setModel(en.trier_nom());
}
// trie id entreprise
void MainWindow::on_supprimer_9_clicked()
{
    ui->table_employe_3->setModel(en.trier_id());
}
// trie secteur entreprise
void MainWindow::on_supprimer_11_clicked()
{
    ui->table_employe_3->setModel(en.trier_secteur());
}
// recherche entreprise
void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    QSqlQueryModel *model= new QSqlQueryModel();
        QSqlQuery   *query= new QSqlQuery();
        query->prepare("SELECT * FROM entreprise WHERE id  LIKE'"+arg1+"%' or nom  LIKE'"+arg1+"%' or adresse  LIKE'"+arg1+"%' or secteur LIKE'"+arg1+"%'");
         query->exec();
         if (query->next()) {
         model->setQuery(*query);
         ui->table_employe_3->setModel(model);
         }
         else {
             QMessageBox::critical(nullptr, QObject::tr("SEARCH"),
                             QObject::tr("NO MATCH FOUND !!\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);
          ui->lineEdit->clear();}
}
// stat entreprise
void MainWindow::on_supprimer_13_clicked()
{
    QSqlQueryModel * model= new QSqlQueryModel();
                                    model->setQuery("select * from entreprise where secteur = 'tech' ");
                                    float e=model->rowCount();
                                    model->setQuery("select * from entreprise where secteur = 'argicole' ");
                                    float ee=model->rowCount();
                                    model->setQuery("select * from entreprise where secteur = 'transport' ");
                                    float eee=model->rowCount();
                                    model->setQuery("select * from entreprise where secteur = 'autre' ");
                                    float eeee=model->rowCount();
                                    float total=e+ee+eee+eeee;
                                    QString a=QString("tech"+QString::number((e*100)/total,'f',2)+"%" );
                                    QString b=QString("agricole"+QString::number((ee*100)/total,'f',2)+"%" );
                                    QString c=QString("transport"+QString::number((eee*100)/total,'f',2)+"%" );
                                    QString d=QString("autre"+QString::number((eeee*100)/total,'f',2)+"%" );
                                    QPieSeries *series = new QPieSeries();
                                    series->append(a,e);
                                    series->append(b,ee);
                                    series->append(c,eee);
                                    series->append(d,eeee);
                            if (e!=0)
                            {QPieSlice *slice = series->slices().at(0);
                             slice->setLabelVisible();
                             slice->setPen(QPen());}
                            if ( ee!=0)
                            {
                                     // Add label, explode and define brush for 2nd slice
                                     QPieSlice *slice1 = series->slices().at(1);
                                     //slice1->setExploded();
                                     slice1->setLabelVisible();
                            }
                            if ( eee!=0)
                            {
                                     // Add label, explode and define brush for 2nd slice
                                     QPieSlice *slice2 = series->slices().at(2);
                                     //slice1->setExploded();
                                     slice2->setLabelVisible();
                            }
                            if ( eeee!=0)
                            {
                                     // Add label, explode and define brush for 2nd slice
                                     QPieSlice *slice3 = series->slices().at(3);
                                     //slice1->setExploded();
                                     slice3->setLabelVisible();
                            }
                                    // Create the chart widget
                                    QChart *chart = new QChart();
                                    // Add data to chart with title and hide legend
                                    chart->addSeries(series);
                                    chart->setTitle("Entreprise par secteur : nombre total des entreprises: "+ QString::number(total));
                                    chart->legend()->hide();
                                    // Used to display the chart
                                    QChartView *chartView = new QChartView(chart);
                                    chartView->setRenderHint(QPainter::Antialiasing);
                                    chartView->resize(1000,500);
                                    chartView->show();
}
// pdf
void MainWindow::on_supprimer_12_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
     qDebug()<<dir;
     QPdfWriter pdf(dir+"/PdfList.pdf");
                            QPainter painter(&pdf);
                           int i = 4000;

                                painter.drawPixmap(QRect(100,100,2500,2500),QPixmap("C:/"));
                                painter.drawText(900,650,"Entreprise");
                                painter.setPen(Qt::red);
                                painter.setFont(QFont("Time New Roman", 25));
                                painter.drawText(3000,1400,"Liste des entreprises");
                                painter.setPen(Qt::black);
                                painter.setFont(QFont("Time New Roman", 15));
                                painter.drawRect(100,100,9400,2500); // dimension taa rectangle li fih liste
                                painter.drawRect(100,3000,9400,500);
                                painter.setFont(QFont("Time New Roman", 9));
                                painter.drawText(300,3300,"ID");
                                painter.drawText(2000,3300,"Nom");
                                painter.drawText(4000,3300,"Adresse");
                                painter.drawText(5600,3300,"Secteur");
                                painter.drawRect(100,3000,9400,10700);
                                //QTextDocument previewDoc;
                                QString pdflist = QDate::currentDate().toString("'data_'MM_dd_yyyy'.txt'");
                                //QTextCursor cursor(&previewDoc);
                                QSqlQuery query;
                                query.prepare("select * from entreprise");
                                query.exec();
                                while (query.next())
                                {
                                    painter.drawText(300,i,query.value(0).toString());
                                    painter.drawText(2000,i,query.value(1).toString());
                                    painter.drawText(4000,i,query.value(2).toString());
                                    painter.drawText(5600,i,query.value(3).toString());
                                   i = i +500;
                                }
                                int reponse = QMessageBox::question(this, "Générer PDF", "<PDF Enregistré>...Vous Voulez Affichez Le PDF ?",
                                                                    QMessageBox::Yes|QMessageBox::No);
                                    if (reponse == QMessageBox::Yes)
                                    {
                                        QDesktopServices::openUrl(QUrl::fromLocalFile(dir+"/PdfList.pdf"));

                                        painter.end();
                                    }
                                    else
                                    {
                                         painter.end();
                                    }
 }
// qr code
void MainWindow::on_supprimer_14_clicked()
{
                  int tabeq=ui->table_employe_3->currentIndex().row();
                  QVariant idd=ui->table_employe_3->model()->data(ui->table_employe_3->model()->index(tabeq,0));
                  QString id=idd.toString();
                 // QString code=idd.toSTring();
                  QSqlQuery qry;
                  qry.prepare("select * from entreprise where id=:code");
                  qry.bindValue(":code",id);
                  qry.exec();

                   QString nom,adresse,secteur;//attributs

                 while(qry.next()){

                     id=qry.value(0).toString();
                     nom=qry.value(1).toString();
                     adresse=qry.value(2).toInt();
                     secteur=qry.value(3).toInt();
                  }
                  id=QString(id);
                         id="CODE:\t" +id+ "NOM\t:" +nom+ "adresse:\t" +adresse+ "secteur:\t" +secteur;
                  QrCode qr = QrCode::encodeText(id.toUtf8().constData(), QrCode::Ecc::HIGH);

                  // Read the black & white pixels
                  QImage im(qr.getSize(),qr.getSize(), QImage::Format_RGB888);
                  for (int y = 0; y < qr.getSize(); y++) {
                      for (int x = 0; x < qr.getSize(); x++) {
                          int color = qr.getModule(x, y);  // 0 for white, 1 for black

                          // You need to modify this part
                          if(color==0)
                              im.setPixel(x, y,qRgb(254, 254, 254));
                          else
                              im.setPixel(x, y,qRgb(0, 0, 0));
                      }
                  }
                  im=im.scaled(200,200);
                 ui->label_3->setPixmap(QPixmap::fromImage(im));
}
// prendre photo employee
void MainWindow::on_supprimer_16_clicked()
{
/*
            // Create a camera object
            camera = new QCamera(this);

            // Create a viewfinder to display the video
            viewfinder = new QCameraViewfinder(this);
            camera->setViewfinder(viewfinder);

            // Create an image capture object
            imageCapture = new QCameraImageCapture(camera, this);

            // Create a layout for the UI elements
            QVBoxLayout *layout = new QVBoxLayout;
            layout->addWidget(viewfinder);

            // Create a button to capture the image
            QPushButton *captureButton = new QPushButton("Capture Image", this);
            layout->addWidget(captureButton);

            // Connect the button to capture the image
            connect(captureButton, &QPushButton::clicked, this, &MainWindow::captureImage);

            // Create a widget to contain the layout
            QWidget *widget = new QWidget(this);
            widget->setLayout(layout);
            setCentralWidget(widget);

            // Start the camera
            camera->start();
*/
}


