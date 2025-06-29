//ZEHRA KUTLUGÜN
//22100011002
#include "mainwindow.h"
#include "ui_mainwindow.h"
// Ödünç alma ve teslim etme işlemlerinde veritabanından tabloları okurken parametre hatası(parameter count mismatch) hatası aldım.
//Farklı yöntemler denesem de son olarak sql deki tabloların adını değiştirdim ve hata düzeldi.
//Bu yüzden odunc_alma tablosu->odunc ; odunc_teslim_etme->deneme olarak değiştirdim.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");
    if(db.open())
    {
        ui->statusbar->showMessage("Veritabanına bağlandı");
    }
    else
    {

        qDebug() << "Veritabanına bağlanamadı:" << db.lastError().text();
        ui->statusbar->showMessage("Veritabanına bağlanamadı");
    }

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{

    Uyeislemleri uyeislemleriDialog(db, this);
    uyeislemleriDialog.exec();
}



void MainWindow::on_btn_kitapislemleri_clicked()
{
    Kitapislemleri kitapislemleriDialog(db, this);
    kitapislemleriDialog.exec();
}


void MainWindow::on_btn_oduncalma_clicked()
{
    Oduncalmaislemleri OduncalmaislemleriDialog(db,this);
    OduncalmaislemleriDialog.exec();

}


void MainWindow::on_btn_oduncteslim_clicked()
{
    Oduncteslimetmeislemleri OduncteslimetmeislemleriDialog(db,this);
    OduncteslimetmeislemleriDialog.exec();
}

