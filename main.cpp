#include "mainwindow.h"
#include "dbmanager.h"
#include <QApplication>
#include "apimanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    ApiManager apiManager;

    // ApiManager sinyalini MainWindow slotuna bağla
    QObject::connect(&apiManager, &ApiManager::apiDataProcessed, &mainWindow, &MainWindow::updatePrayerTimes);

    DBManager db;
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime targetDateTime = currentTime.addDays(7);
    QString date = currentTime.toString("dd-MM-yyyy");
    QString targetDate = targetDateTime.toString("dd-MM-yyyy");
    QVector<DBManager::prayerTimeStruct> times = db.getPrayerTimesList(date, targetDate);
    if (times.isEmpty()) {
        QString urlBase = "http://api.aladhan.com/v1/calendarByCity/2023/%1?city=Bursa&country=Turkey&method=2";
        urlBase = urlBase.arg(currentTime.toString("MM"));

        QUrl apiUrl(urlBase); // Örnek API URL
        apiManager.makeApiRequest(apiUrl);
        return a.exec();
    }

    MainWindow w;
    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();
    return a.exec();
}
