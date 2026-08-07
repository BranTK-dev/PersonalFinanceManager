#include "FileManager.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QString FileManager::defaultFilePath()
{
    return QStringLiteral("data/transactions.json");
}

bool FileManager::saveToFile(const QString &path, const QVector<Transaction> &transactions)
{
    // Make sure the containing folder exists (matters if the app is run from a fresh checkout)
    QDir dir = QFileInfo(path).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QJsonArray array;
    for (const Transaction &t : transactions) {
        array.append(t.toJson());
    }

    QJsonDocument doc(array);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool FileManager::loadFromFile(const QString &path, QVector<Transaction> &outTransactions)
{
    QFile file(path);
    if (!file.exists()) {
        // Normal on first run, there's nothing to load yet.
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        return false;
    }

    QVector<Transaction> result;
    QJsonArray array = doc.array();
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            result.append(Transaction::fromJson(value.toObject()));
        }
    }

    outTransactions = result;
    return true;
}
