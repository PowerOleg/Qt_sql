#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>
#include <QModelIndex>
#include <QDebug>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

#define NUM_DATA_FOR_CONNECT_TO_DB 5//Количество полей данных необходимых для подключения к БД

enum FieldsForConnect
{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

const static int NUM_COLUMS = 2;

enum RequestType
{
    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3
};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(int film_category, QString request);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);


signals:
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlQueryModel* model);

private:
    QSqlDatabase* dataBase;
    QSqlQuery* simpleQuery;
    QTableWidget* tableWidget;
    QSqlTableModel* model;
    QSqlQueryModel* queryModel;

};

#endif // DATABASE_H
