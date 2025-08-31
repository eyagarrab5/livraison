#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
   db.setUserName("mejjri");
   db.setPassword("mejri3");

if (db.open())
test=true;





    return  test;
}
