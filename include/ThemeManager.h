#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

class ThemeManager
{
public:
    // Applies the dark or light palette to the whole application immediately.
    static void applyTheme(bool dark);

    // Reads the last-used preference from disk. Defaults to light (false) on first run.
    static bool isDarkModeEnabled();

    // Saves the preference to disk and applies it right away.
    static void setDarkModeEnabled(bool enabled);
};

#endif // THEMEMANAGER_H
