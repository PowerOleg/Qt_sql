#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    this->dataBase = new QSqlDatabase();
    this->queryModel = new QSqlQueryModel;
    this->tableWidget = new QTableWidget();
}

DataBase::~DataBase()
{
    delete dataBase;
    delete model;
    delete queryModel;
    delete tableWidget;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{
    if (data[hostName] != "")
    {
        dataBase->setHostName(data[hostName]);
        dataBase->setDatabaseName(data[dbName]);
        dataBase->setUserName(data[login]);
        dataBase->setPassword(data[pass]);
        dataBase->setPort(data[port].toInt());
    }
    else
    {
        //mock to get data for connection
        dataBase->setHostName("981757-ca08998.tmweb.ru");
        dataBase->setDatabaseName("netology_cpp");
        dataBase->setUserName("netology_usr_cpp");
        dataBase->setPassword("CppNeto3");
        dataBase->setPort(5432);
    }

    bool status;
    status = dataBase->open();
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(int film_category, QString request)
{
    //пример использования QSqlQueryModel
    if (film_category == RequestType::requestHorrors || film_category == RequestType::requestComedy)
    {
        this->queryModel->setQuery(request, *dataBase);
        this->queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        this->queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendStatusRequest(this->queryModel);
    }

    //пример использования TableModel
    if (film_category == RequestType::requestAllFilms)
    {
        this->model = new QSqlTableModel(this, *dataBase);
        this->model->setTable("film");
        this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        this->model->select();
        emit sig_SendStatusRequest(this->model);
    }

}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
