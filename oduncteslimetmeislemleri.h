//ZEHRA KUTLUGÜN
//22100011002
#ifndef ODUNCTESLIMETMEISLEMLERI_H
#define ODUNCTESLIMETMEISLEMLERI_H

#include <QDialog>
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSql>
#include<QtSql/QSqlQueryModel>
#include<QMessageBox>
#include<QSqlQuery>
#include<QVariant>
#include <QSqlTableModel>
#include<QSqlError>
namespace Ui {
class Oduncteslimetmeislemleri;
}

class Oduncteslimetmeislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit Oduncteslimetmeislemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~Oduncteslimetmeislemleri();

    void listele2();
public slots:
    void oduncAlinanKitabaTiklandi(const QModelIndex &index);
private slots:
    void on_btn_oduncuver_clicked();

private:
    Ui::Oduncteslimetmeislemleri *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("SQLİTE");
    QSqlQuery *sorgu;
    QSqlQueryModel *model1,*model2,*model3,*model4;
    void listele();
};

#endif // ODUNCTESLIMETMEISLEMLERI_H
