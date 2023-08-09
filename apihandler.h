// apihandler.h
#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// apimanager.h
class ApiManager : public QObject
{
    Q_OBJECT

public:
    explicit ApiManager(QObject *parent = nullptr);

    // API isteği yapacak fonksiyon
    void makeApiRequest();

signals:
    void apiReplyReceived(const QByteArray &response);

private:
    QNetworkAccessManager *networkManager;
};


#endif // APIHANDLER_H
