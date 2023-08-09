#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "apimanager.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updatePrayerTimes();
    void updateTime();
    void checkTime();
    void calculateTime(QWidget *widget, QString title, QTime nextTime);



private:
    Ui::MainWindow *ui;
    void setLabel(int size, bool bold, const QString text, const QString color, QLabel *label, Qt::Alignment flag);
    QVector<QTime> todayPrayerTime;

    void addPrayerTime(QString value);

    QVector<QTime> getPrayerTimes();
};
#endif // MAINWINDOW_H
