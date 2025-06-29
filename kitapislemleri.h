//ZEHRA KUTLUGÜN
//22100011002
#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

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
class Kitapislemleri;
}

class Kitapislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit Kitapislemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~Kitapislemleri();
    void listele();

public slots:
    void kitabaTiklandi(const QModelIndex &index);
private slots:
    void on_btn_yenikayit_clicked();

    void on_btn_guncelle_clicked();

    void on_btn_sil_clicked();

private:

    Ui::Kitapislemleri *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("SQLİTE");
    QSqlQuery *sorgu;
    QSqlQueryModel *model1,*model2,*model3;
};

#endif // KITAPISLEMLERI_H
