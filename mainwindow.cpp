#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);
    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData)
    {
        dataForConnect = receivData;
    });
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");


       auto conn = [this]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else
    {
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    if (ui->cb_category->currentIndex() + 1 == RequestType::requestHorrors)
    {
        request = request_horror_category;
    }
    if (ui->cb_category->currentIndex() + 1 == RequestType::requestComedy)
    {
        request = request_comedy_category;
    }
    auto request_to_db = [this]{dataBase->RequestToDB(ui->cb_category->currentIndex() + 1, request);};
    QtConcurrent::run(request_to_db);

}

/*!
 * \brief Метод изменяет состояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if (status)
    {
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else
    {
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }
}

void MainWindow::ReceiveStatusRequestToDB(QSqlQueryModel* model)
{
    const QSqlError err = model->lastError();
    if (err.isValid())
    {
        msg->setText(err.text());
        msg->show();
    }
    else
    {
        ui->tv_result->setModel(model);
        if (ui->cb_category->currentIndex() == 0)
        {
            ui->tv_result->hideColumn(0);
        } else
        {
             ui->tv_result->showColumn(0);
        }

    }
}

void MainWindow::on_pb_clear_clicked()
{
    ui->tv_result->setModel(new QSqlQueryModel);
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
