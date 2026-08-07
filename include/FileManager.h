#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QVector>
#include <QString>
#include "Transaction.h"

class FileManager
{
public:
    // Default location, relative to the app's working directory, e.g. data/transactions.json
    static QString defaultFilePath();

    // Writes all transactions to a JSON file. Returns false if the file couldn't be written.
    static bool saveToFile(const QString &path, const QVector<Transaction> &transactions);

    // Reads transactions from a JSON file into 'outTransactions'.
    // Returns false if the file doesn't exist or isn't valid JSON.
    // A missing file on first run is expected, not an error the caller needs to alarm the user about.
    static bool loadFromFile(const QString &path, QVector<Transaction> &outTransactions);
};

#endif // FILEMANAGER_H
