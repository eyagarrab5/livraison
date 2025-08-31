#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("SourceProjet_2A");
   db.setUserName("eya");
   db.setPassword("eya123");

if (db.open())
test=true;





    return  test;
}
