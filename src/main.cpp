#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QResource>
#include <QTranslator>

#include "danmuku.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName =
            "bilibili-live-danmuku_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QFile qss(":/stylesheet/display.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("QSS loaded.");
        QString stylesheet = QLatin1String(qss.readAll());
        a.setStyleSheet(stylesheet);
        qss.close();
    } else {
        qWarning("Failed to load QSS stylesheet (display.qss).");
    }

    Danmuku w;
    w.show();
    return a.exec();
}