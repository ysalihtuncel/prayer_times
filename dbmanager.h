#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include "apimanager.h"


class DBManager : public QObject
{
    Q_OBJECT
public:
    struct prayerTimeStruct{
        QString id = "id";
        QString date = "date";
        QString weekday = "weekday";
        QString month = "month";
        QString hijriDate = "hijriDate";
        QString hijriWeekday = "hijriWeekday";
        QString hijriMonth = "hijriMonth";
        QString Imsak = "Imsak";
        QString Sunrise = "Sunrise";
        QString Dhuhr = "Dhuhr";
        QString Asr = "Asr";
        QString Maghrib = "Maghrib";
        QString Isha = "Isha";
    };
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();
    QVector<prayerTimeStruct> getPrayerTimesList(const QString &startDate, const QString &endDate);
    bool createTable();
    bool insertData(prayerTimeStruct prayerTime);

private:
    QSqlDatabase db;
    QString databaseName;

};

#endif // DBMANAGER_H
