#include "ThemeManager.h"

#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QSettings>
#include <QStyleFactory>
#include <QStyle>

namespace {
    const char *SETTINGS_ORG = "PersonalFinanceManager";
    const char *SETTINGS_APP = "PersonalFinanceManager";
    const char *DARK_MODE_KEY = "darkModeEnabled";
}

void ThemeManager::applyTheme(bool dark)
{
    // Capture the app's true native style/palette exactly once, on whichever call happens
    // first (normally at startup). That's what we restore for "light mode" - not Fusion's
    // own light palette, which looks flat and gray compared to your OS's native look.
    static QString nativeStyleName;
    static QPalette nativePalette;
    static bool captured = false;
    if (!captured) {
        nativeStyleName = qApp->style()->objectName();
        nativePalette = qApp->palette();
        captured = true;
    }

    if (!dark) {
        qApp->setStyle(QStyleFactory::create(nativeStyleName));
        qApp->setPalette(nativePalette);
        return;
    }

    // Fusion is the style that reliably respects QPalette colors across platforms;
    // native styles (especially on Windows) often ignore palette changes for dark mode.
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(37, 37, 38));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 46));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 46));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(100, 170, 255));
    darkPalette.setColor(QPalette::Highlight, QColor(60, 110, 165));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

    qApp->setPalette(darkPalette);
}

bool ThemeManager::isDarkModeEnabled()
{
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(DARK_MODE_KEY, false).toBool();
}

void ThemeManager::setDarkModeEnabled(bool enabled)
{
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(DARK_MODE_KEY, enabled);
    applyTheme(enabled);
}
