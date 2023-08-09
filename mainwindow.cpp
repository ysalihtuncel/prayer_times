#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apimanager.h"
#include "dbmanager.h"
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(40, 143, 167);");
    this->setFixedSize(1150,860);
    ui->widgetParent->setStyleSheet("background-color: rgb(17, 131, 157);");
    ui->widgetTop->setStyleSheet("background-color: rgb(17, 131, 157);");
    ui->weekWidget->setStyleSheet("background-color: rgb(255, 255, 255);");

    DBManager db;
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime targetDateTime = currentTime.addDays(7);
    QString date = currentTime.toString("dd-MM-yyyy");
    QString targetDate = targetDateTime.toString("dd-MM-yyyy");
    QVector<DBManager::prayerTimeStruct> times = db.getPrayerTimesList(date, targetDate);
    if (!times.isEmpty()) {
        DBManager::prayerTimeStruct day = times.first();
        setLabel(16, false, currentTime.toString("d") + " " + day.month + " " + day.weekday, "color: rgb(255, 255, 255);", ui->dateLabel, Qt::AlignCenter);

        QDateTime hijriDate = QDateTime::fromString(day.hijriDate, "dd-MM-yyyy");
        QString hijriYear = hijriDate.toString("yyyy");
        QString hijriDay = hijriDate.toString("d");

        setLabel(16, false, hijriDay + " " + day.hijriMonth + " " + hijriYear, "color: rgb(248, 240, 171);", ui->hijriDateLabel, Qt::AlignCenter);

        setLabel(18, true, day.Imsak, "color: rgb(255, 255, 255);", ui->imsakLabel, Qt::AlignCenter);
        setLabel(18, true, day.Sunrise, "color: rgb(255, 255, 255);", ui->sunriseLabel, Qt::AlignCenter);
        setLabel(18, true, day.Dhuhr, "color: rgb(255, 255, 255);", ui->dhuhrLabel, Qt::AlignCenter);
        setLabel(18, true, day.Asr, "color: rgb(255, 255, 255);", ui->asrLabel, Qt::AlignCenter);
        setLabel(18, true, day.Maghrib, "color: rgb(255, 255, 255);", ui->maghribLabel, Qt::AlignCenter);
        setLabel(18, true, day.Isha, "color: rgb(255, 255, 255);", ui->ishaLabel, Qt::AlignCenter);

        addPrayerTime(day.Imsak);
        addPrayerTime(day.Sunrise);
        addPrayerTime(day.Dhuhr);
        addPrayerTime(day.Asr);
        addPrayerTime(day.Maghrib);
        addPrayerTime(day.Isha);

        QFont newFont;
        newFont.setFamily("Arial");
        newFont.setPointSize(10);

        foreach(const DBManager::prayerTimeStruct time, times) {
            QHBoxLayout *horVertical = new QHBoxLayout();

            QDateTime date = QDateTime::fromString(time.date, "dd-MM-yyyy");
            QString year = date.toString("yyyy");
            QString day = date.toString("dd");

            QDateTime hijriDate = QDateTime::fromString(time.hijriDate, "dd-MM-yyyy");
            QString hijriYear = hijriDate.toString("yyyy");
            QString hijriDay = hijriDate.toString("dd");

            QLabel *dateLabel = new QLabel(day + " " + time.month + " " + year);
            dateLabel->setFont(newFont);
            dateLabel->setStyleSheet("color: black;");
            dateLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(dateLabel);

            QLabel *hijriDateLabel = new QLabel(hijriDay + " " + time.hijriMonth + " " + hijriYear);
            hijriDateLabel->setFont(newFont);
            hijriDateLabel->setStyleSheet("color: black;");
            hijriDateLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(hijriDateLabel);

            QLabel *imsakLabel = new QLabel(time.Imsak);
            imsakLabel->setFont(newFont);
            imsakLabel->setStyleSheet("color: black;");
            imsakLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(imsakLabel);

            QLabel *sunriseLabel = new QLabel(time.Sunrise);
            sunriseLabel->setFont(newFont);
            sunriseLabel->setStyleSheet("color: black;");
            sunriseLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(sunriseLabel);

            QLabel *dhurLabel = new QLabel(time.Dhuhr);
            dhurLabel->setFont(newFont);
            dhurLabel->setStyleSheet("color: black;");
            dhurLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(dhurLabel);

            QLabel *asrLabel = new QLabel(time.Asr);
            asrLabel->setFont(newFont);
            asrLabel->setStyleSheet("color: black;");
            asrLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(asrLabel);

            QLabel *maghribLabel = new QLabel(time.Maghrib);
            maghribLabel->setFont(newFont);
            maghribLabel->setStyleSheet("color: black;");
            maghribLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(maghribLabel);

            QLabel *ishaLabel = new QLabel(time.Isha);
            ishaLabel->setFont(newFont);
            ishaLabel->setStyleSheet("color: black;");
            ishaLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(ishaLabel);

            ui->weekVerticalLayout->addLayout(horVertical);
        }
    }



    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::checkTime(){
    QString _time = QDateTime::currentDateTime().toString("hh:mm:ss");

    QTime currentTime = QTime::fromString(_time);

    QVector<QTime> prayerTimesVector = getPrayerTimes();

    if (prayerTimesVector.size() == 0) return;
    ui->imsakWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");
    ui->sunriseWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");
    ui->dhuhrWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");
    ui->ishaWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");
    ui->asrWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");
    ui->maghribWidget->setStyleSheet("background-color: rgba(103, 188, 154, 0);");

    if (currentTime < prayerTimesVector[0]) {
        calculateTime(ui->ishaWidget, "İmsak", prayerTimesVector[0]);

        int minutesDifference = currentTime.secsTo(prayerTimesVector[0]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[0]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[0]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Imsak vaktine 15 dakika kaldı");
        }
    }
    else if(currentTime >= prayerTimesVector[0] && currentTime < prayerTimesVector[1]) {
        //Imsak
        calculateTime(ui->imsakWidget, "Güneş", prayerTimesVector[1]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[1]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[1]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[1]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Güneş vaktine 15 dakika kaldı");
        }
    }
    else if (currentTime >= prayerTimesVector[1] && currentTime < prayerTimesVector[2]) {
        //Sunrise
        calculateTime(ui->sunriseWidget, "Öğle", prayerTimesVector[2]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[2]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[2]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[2]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Öğle vaktine 15 dakika kaldı");
        }

    }
    else if (currentTime >= prayerTimesVector[2] && currentTime < prayerTimesVector[3]) {
        //Dhuhr
        calculateTime(ui->dhuhrWidget, "İkindi", prayerTimesVector[3]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[3]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[3]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[3]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "İkindi vaktine 15 dakika kaldı");
        }

    }
    else if (currentTime >= prayerTimesVector[3] && currentTime < prayerTimesVector[4]) {
        //Asr
        calculateTime(ui->asrWidget, "Akşam", prayerTimesVector[4]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[4]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[4]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[4]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Akşam vaktine 15 dakika kaldı");
        }

    }
    else if (currentTime >= prayerTimesVector[4] && currentTime < prayerTimesVector[5]) {
        //Maghrib
        calculateTime(ui->maghribWidget, "Yatsı", prayerTimesVector[5]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[5]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[5]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[5]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Yatsı vaktine 15 dakika kaldı");
        }

    }
    else if (currentTime >= prayerTimesVector[5]) {
        //Isha
        calculateTime(ui->ishaWidget, "İmsak", prayerTimesVector[0]);
        int minutesDifference = currentTime.secsTo(prayerTimesVector[0]) / 60; // İki zaman arasındaki farkı dakika cinsinden hesapla
        qDebug() << minutesDifference << " " << currentTime.secsTo(prayerTimesVector[0]) % 60;
        if (minutesDifference == 15 && currentTime.secsTo(prayerTimesVector[0]) % 60 == 0) {
            QMessageBox::warning(this, "Uyarı", "Imsak vaktine 15 dakika kaldı");
        }
    }

}

void MainWindow::calculateTime(QWidget *widget, QString title, QTime nextTime) {
    QString _time = QDateTime::currentDateTime().toString("hh:mm:ss");

    widget->setStyleSheet("background-color: rgb(103, 188, 154);");
    QTime currTime = QTime::fromString(_time);
    QString titleLabel = "%1 Vaktine Kalan Süre";
    titleLabel = titleLabel.arg(title);
    QFont newFont;
    newFont.setFamily("Arial");
    newFont.setPointSize(12);
    newFont.setBold(true);
    ui->remainingTitleLabel->setText(titleLabel);
    ui->remainingTitleLabel->setStyleSheet("color: white;");
    ui->remainingTitleLabel->setFont(newFont);
    ui->remainingTitleLabel->setAlignment(Qt::AlignCenter);

    int timeDiff = currTime.msecsTo(nextTime);
    int hours = timeDiff / (1000 * 60 * 60);
    int minutes = (timeDiff % (1000 * 60 * 60)) / (1000 * 60);
    int seconds = (timeDiff % (1000 * 60)) / 1000;

    QString hoursString = (hours<10) ? "0%1" : "%1";
    hoursString = hoursString.arg(hours);
    QString minutesString = (minutes<10) ? "0%1" : "%1";
    minutesString = minutesString.arg(minutes);
    QString secondsString = (seconds<10) ? "0%1" : "%1";
    secondsString = secondsString.arg(seconds);
    QString finalString = hoursString + ":" + minutesString + ":" + secondsString;

    ui->remainingLabel->setText(finalString);
    newFont.setPointSize(20);
    newFont.setBold(false);
    ui->remainingLabel->setFont(newFont);
    ui->remainingLabel->setStyleSheet("color: rgb(248, 240, 171);");
    ui->remainingLabel->setAlignment(Qt::AlignCenter);
}

void MainWindow::updatePrayerTimes() {
    this->setWindowIcon(QIcon(":/icon.png"));
    this->show();
    DBManager db;
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime targetDateTime = currentTime.addDays(7);
    QString date = currentTime.toString("dd-MM-yyyy");
    QString targetDate = targetDateTime.toString("dd-MM-yyyy");
    QVector<DBManager::prayerTimeStruct> times = db.getPrayerTimesList(date, targetDate);
    if (!times.isEmpty()) {
        DBManager::prayerTimeStruct day = times.first();
        setLabel(16, false, currentTime.toString("d") + " " + day.month + " " + day.weekday, "color: rgb(255, 255, 255);", ui->dateLabel, Qt::AlignCenter);

        QDateTime hijriDate = QDateTime::fromString(day.hijriDate, "dd-MM-yyyy");
        QString hijriYear = hijriDate.toString("yyyy");
        QString hijriDay = hijriDate.toString("d");

        setLabel(16, false, hijriDay + " " + day.hijriMonth + " " + hijriYear, "color: rgb(248, 240, 171);", ui->hijriDateLabel, Qt::AlignCenter);

        setLabel(18, true, day.Imsak, "color: rgb(255, 255, 255);", ui->imsakLabel, Qt::AlignCenter);
        setLabel(18, true, day.Sunrise, "color: rgb(255, 255, 255);", ui->sunriseLabel, Qt::AlignCenter);
        setLabel(18, true, day.Dhuhr, "color: rgb(255, 255, 255);", ui->dhuhrLabel, Qt::AlignCenter);
        setLabel(18, true, day.Asr, "color: rgb(255, 255, 255);", ui->asrLabel, Qt::AlignCenter);
        setLabel(18, true, day.Maghrib, "color: rgb(255, 255, 255);", ui->maghribLabel, Qt::AlignCenter);
        setLabel(18, true, day.Isha, "color: rgb(255, 255, 255);", ui->ishaLabel, Qt::AlignCenter);

        addPrayerTime(day.Imsak);
        addPrayerTime(day.Sunrise);
        addPrayerTime(day.Dhuhr);
        addPrayerTime(day.Asr);
        addPrayerTime(day.Maghrib);
        addPrayerTime(day.Isha);

        QFont newFont;
        newFont.setFamily("Arial");
        newFont.setPointSize(10);

        foreach(const DBManager::prayerTimeStruct time, times) {
            QHBoxLayout *horVertical = new QHBoxLayout();

            QDateTime date = QDateTime::fromString(time.date, "dd-MM-yyyy");
            QString year = date.toString("yyyy");
            QString day = date.toString("dd");

            QDateTime hijriDate = QDateTime::fromString(time.hijriDate, "dd-MM-yyyy");
            QString hijriYear = hijriDate.toString("yyyy");
            QString hijriDay = hijriDate.toString("dd");

            QLabel *dateLabel = new QLabel(day + " " + time.month + " " + year);
            dateLabel->setFont(newFont);
            dateLabel->setStyleSheet("color: black;");
            dateLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(dateLabel);

            QLabel *hijriDateLabel = new QLabel(hijriDay + " " + time.hijriMonth + " " + hijriYear);
            hijriDateLabel->setFont(newFont);
            hijriDateLabel->setStyleSheet("color: black;");
            hijriDateLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(hijriDateLabel);

            QLabel *imsakLabel = new QLabel(time.Imsak);
            imsakLabel->setFont(newFont);
            imsakLabel->setStyleSheet("color: black;");
            imsakLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(imsakLabel);

            QLabel *sunriseLabel = new QLabel(time.Sunrise);
            sunriseLabel->setFont(newFont);
            sunriseLabel->setStyleSheet("color: black;");
            sunriseLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(sunriseLabel);

            QLabel *dhurLabel = new QLabel(time.Dhuhr);
            dhurLabel->setFont(newFont);
            dhurLabel->setStyleSheet("color: black;");
            dhurLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(dhurLabel);

            QLabel *asrLabel = new QLabel(time.Asr);
            asrLabel->setFont(newFont);
            asrLabel->setStyleSheet("color: black;");
            asrLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(asrLabel);

            QLabel *maghribLabel = new QLabel(time.Maghrib);
            maghribLabel->setFont(newFont);
            maghribLabel->setStyleSheet("color: black;");
            maghribLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(maghribLabel);

            QLabel *ishaLabel = new QLabel(time.Isha);
            ishaLabel->setFont(newFont);
            ishaLabel->setStyleSheet("color: black;");
            ishaLabel->setAlignment(Qt::AlignCenter);
            horVertical->addWidget(ishaLabel);

            ui->weekVerticalLayout->addLayout(horVertical);
        }
    }
}

void MainWindow::updateTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    setLabel(20, false, currentTime.toString("hh:mm"), "color: rgb(248, 240, 171);", ui->timeLabel, Qt::AlignCenter);
    checkTime();
}

void MainWindow::setLabel(int size, bool bold, const QString text, const QString color, QLabel *label, Qt::Alignment flag) {
    label->setText(text);
    QFont newFont;
    newFont.setFamily("Arial");
    newFont.setPointSize(size);
    newFont.setBold(bold);
    label->setFont(newFont);
    label->setStyleSheet(color);
    label->setAlignment(flag);
}

void MainWindow::addPrayerTime(QString value) {
    if (todayPrayerTime.count() >= 6) {
        todayPrayerTime.clear();
    }
    todayPrayerTime.append(QTime::fromString(value, "hh:mm"));
}

QVector<QTime> MainWindow::getPrayerTimes() {
    return todayPrayerTime;
}
