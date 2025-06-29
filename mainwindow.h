//ZEHRA KUTLUGÜN
//22100011002
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSql>
#include<QtSql/QSqlQueryModel>
#include<QMessageBox>
#include<QSqlQuery>
#include<QSqlTableModel>
#include <QSqlError>
#include<uyeislemleri.h>
#include<kitapislemleri.h>
#include<oduncalmaislemleri.h>
#include<oduncteslimetmeislemleri.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pushButton_clicked();

    void on_btn_kitapislemleri_clicked();

    void on_btn_oduncalma_clicked();

    void on_btn_oduncteslim_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("SQLİTE");
    QSqlQuery *sorgu;
    QSqlQueryModel *model1,*model2;
    void listele();

};
#endif // MAINWINDOW_H
