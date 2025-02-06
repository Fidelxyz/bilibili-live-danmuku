#include <QApplication>
#include <QTranslator>

#include "danmuku.h"

int main(int argc, char *argv[]) {
    qSetMessagePattern(
        "%{if-category}%{category} | %{endif}%{type} | %{file}:%{line} | "
        "%{function} | %{message}");

    QApplication a(argc, argv);

    QTranslator       translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        if (const QString baseName = QLocale(locale).name();
            translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    const auto w = new Danmuku();
    w->show();
    return QApplication::exec();
}