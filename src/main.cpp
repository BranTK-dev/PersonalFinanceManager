#include <QApplication>
#include <QIcon>
#include "MainWindow.h"
#include "ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/icon_256.png"));

    ThemeManager::applyTheme(ThemeManager::isDarkModeEnabled());

    MainWindow window;
    window.show();

    return app.exec();
}
