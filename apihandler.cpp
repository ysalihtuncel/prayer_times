// apihandler.cpp
#include "apihandler.h"

ApiManager::ApiManager(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void ApiManager::makeApiRequest()
{
    QNetworkRequest request(QUrl("http://api.example.com")); // API URL'sini buraya ekleyin
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit apiReplyReceived(reply->readAll());
        } else {
            qDebug() << "API Hatası:" << reply->errorString();
        }

        reply->deleteLater();
    });
}
