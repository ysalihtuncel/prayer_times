#include "apimanager.h"
#include "dbmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include "mainwindow.h"

ApiManager::ApiManager(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ApiManager::onNetworkReply);
}

void ApiManager::makeApiRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    m_networkManager->get(request);
}

void ApiManager::onNetworkReply(QNetworkReply *reply)
{
    qDebug() << "Network reply received.";

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        // JSON verisini işle
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray dataArray = jsonObj.value("data").toArray();
        qDebug() << dataArray.size();
        qDebug() << dataArray.first();
        DBManager dbManager;
        foreach(const QJsonValue &dailyData, dataArray) {
            QJsonObject dailyObj = dailyData.toObject();
            QJsonObject timingsObj = dailyObj.value("timings").toObject();

            // Namaz vakitlerini ayrıştır ve veritabanına kaydet
            QString date = dailyObj.value("date").toObject().value("gregorian").toObject().value("date").toString();
            QString weekday = dailyObj.value("date").toObject().value("gregorian").toObject().value("weekday").toObject().value("en").toString();
            QString month = dailyObj.value("date").toObject().value("gregorian").toObject().value("month").toObject().value("en").toString();

            QString hijriDate = dailyObj.value("date").toObject().value("hijri").toObject().value("date").toString();
            QString hijriWeekday = dailyObj.value("date").toObject().value("hijri").toObject().value("weekday").toObject().value("en").toString();
            QString hijriMonth = dailyObj.value("date").toObject().value("hijri").toObject().value("month").toObject().value("en").toString();

            QString Imsak = timingsObj.value("Imsak").toString();
            QString Sunrise = timingsObj.value("Sunrise").toString();
            QString Dhuhr = timingsObj.value("Dhuhr").toString();
            QString Asr = timingsObj.value("Asr").toString();
            QString Maghrib = timingsObj.value("Maghrib").toString();
            QString Isha = timingsObj.value("Isha").toString();
            DBManager::prayerTimeStruct prayerTime;

            prayerTime.date = date;
            prayerTime.weekday = weekday;
            prayerTime.month = month;
            prayerTime.hijriDate = hijriDate;
            prayerTime.hijriWeekday = hijriWeekday;
            prayerTime.hijriMonth = hijriMonth;
            prayerTime.Imsak = Imsak.split(" ")[0];
            prayerTime.Sunrise = Sunrise.split(" ")[0];
            prayerTime.Dhuhr = Dhuhr.split(" ")[0];
            prayerTime.Asr = Asr.split(" ")[0];
            prayerTime.Maghrib = Maghrib.split(" ")[0];
            prayerTime.Isha = Isha.split(" ")[0];

            bool a = dbManager.insertData(prayerTime);
            qDebug() << date << " " << a;

        }
        qDebug() << "emit";
        emit apiDataProcessed();
    }
    else {
        // API yanıtında hata varsa, hata mesajını işleyebilirsiniz
        qDebug() << "API Hatası:" << reply->errorString();
    }

    // Cevap alındıktan sonra QNetworkReply'ı temizleyin
    reply->deleteLater();
}
