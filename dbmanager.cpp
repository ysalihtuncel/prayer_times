#include "dbmanager.h"
#include "apimanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent) {


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("prayerTimes.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database fail";
    }
    else {

        qDebug() << "Database: connection ok";
    }
    createTable();
}

DBManager::~DBManager() {
    db.close();
}


QVector<DBManager::prayerTimeStruct> DBManager::getPrayerTimesList(const QString &startDate, const QString &endDate)
{
    QVector<prayerTimeStruct> prayerTimes;

    QSqlQuery query;
    query.prepare("SELECT * FROM prayertimes WHERE date BETWEEN :startDate AND :endDate");
    query.bindValue(":startDate", startDate);
    query.bindValue(":endDate", endDate);

    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError();
        return prayerTimes;
    }

    while (query.next()) {
        prayerTimeStruct time;
        time.date = query.value("date").toString();
        time.weekday = query.value("weekday").toString();
        time.month = query.value("month").toString();
        time.hijriDate = query.value("hijriDate").toString();
        time.hijriWeekday = query.value("hijriWeekday").toString();
        time.hijriMonth = query.value("hijriMonth").toString();
        time.Imsak = query.value("Imsak").toString();
        time.Sunrise = query.value("Sunrise").toString();
        time.Dhuhr = query.value("Dhuhr").toString();
        time.Asr = query.value("Asr").toString();
        time.Maghrib = query.value("Maghrib").toString();
        time.Isha = query.value("Isha").toString();


        prayerTimes.append(time);
    }
    return prayerTimes;
}

bool DBManager::createTable() {
    QSqlQuery query;
    prayerTimeStruct prayerTime;
    QString tableName = "prayertimes";

    return query.exec("CREATE TABLE IF NOT EXISTS " + tableName + "("
                      + prayerTime.id + " INTEGER PRIMARY KEY, "
                      + prayerTime.date + " TEXT, "
                      + prayerTime.weekday + " TEXT, "
                      + prayerTime.month + " TEXT, "
                      + prayerTime.hijriDate + " TEXT, "
                      + prayerTime.hijriWeekday + " TEXT, "
                      + prayerTime.hijriMonth + " TEXT, "
                      + prayerTime.Imsak + " TEXT, "
                      + prayerTime.Sunrise + " TEXT, "
                      + prayerTime.Dhuhr + " TEXT, "
                      + prayerTime.Asr + " TEXT, "
                      + prayerTime.Maghrib + " TEXT, "
                      + prayerTime.Isha + " TEXT ); "
                      );
}

bool DBManager::insertData(prayerTimeStruct prayerTime) {
    QSqlQuery query;
    QString tableName = "prayertimes";
    prayerTimeStruct _prayerTime;
    query.prepare("INSERT INTO " + tableName + "("
                  + _prayerTime.date + ","
                  + _prayerTime.weekday + ","
                  + _prayerTime.month + ","
                  + _prayerTime.hijriDate + ","
                  + _prayerTime.hijriWeekday + ","
                  + _prayerTime.hijriMonth + ","
                  + _prayerTime.Imsak + ","
                  + _prayerTime.Sunrise + ","
                  + _prayerTime.Dhuhr + ","
                  + _prayerTime.Asr + ","
                  + _prayerTime.Maghrib + ","
                  + _prayerTime.Isha + ")"
                  "VALUES ("
                  + ":" + _prayerTime.date + ","
                  + ":" + _prayerTime.weekday + ","
                  + ":" + _prayerTime.month + ","
                  + ":" + _prayerTime.hijriDate + ","
                  + ":" + _prayerTime.hijriWeekday + ","
                  + ":" + _prayerTime.hijriMonth + ","
                  + ":" + _prayerTime.Imsak + ","
                  + ":" + _prayerTime.Sunrise + ","
                  + ":" + _prayerTime.Dhuhr + ","
                  + ":" + _prayerTime.Asr + ","
                  + ":" + _prayerTime.Maghrib + ","
                  + ":" + _prayerTime.Isha + ")"
                  );

    query.bindValue(":" + _prayerTime.date, prayerTime.date);
    query.bindValue(":" + _prayerTime.weekday, prayerTime.weekday);
    query.bindValue(":" + _prayerTime.weekday, prayerTime.weekday);
    query.bindValue(":" + _prayerTime.month, prayerTime.month);
    query.bindValue(":" + _prayerTime.hijriDate, prayerTime.hijriDate);
    query.bindValue(":" + _prayerTime.hijriWeekday, prayerTime.hijriWeekday);
    query.bindValue(":" + _prayerTime.hijriMonth, prayerTime.hijriMonth);
    query.bindValue(":" + _prayerTime.Imsak, prayerTime.Imsak);
    query.bindValue(":" + _prayerTime.Sunrise, prayerTime.Sunrise);
    query.bindValue(":" + _prayerTime.Dhuhr, prayerTime.Dhuhr);
    query.bindValue(":" + _prayerTime.Asr, prayerTime.Asr);
    query.bindValue(":" + _prayerTime.Maghrib, prayerTime.Maghrib);
    query.bindValue(":" + _prayerTime.Isha, prayerTime.Isha);

    if (query.exec()) {
            qDebug() << "Veri başarıyla eklendi.";
            return true;
        } else {
            qDebug() << "Veri ekleme hatası:" << query.lastError().text();
            return false;
        }
}
