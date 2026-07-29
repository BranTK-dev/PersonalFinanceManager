#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>
#include <QJsonObject>

enum class TransactionType {
    Income,
    Expense
};

class Transaction
{
public:
    Transaction();
    Transaction(int id, TransactionType type, double amount, const QString &category,
                const QString &description, const QDate &date);

    // Getters
    int id() const;
    TransactionType type() const;
    double amount() const;
    QString category() const;
    QString description() const;
    QDate date() const;

    // Setters
    void setId(int id);
    void setType(TransactionType type);
    void setAmount(double amount);
    void setCategory(const QString &category);
    void setDescription(const QString &description);
    void setDate(const QDate &date);

    // Helpers
    QString typeToString() const;
    static TransactionType stringToType(const QString &typeStr);

    // JSON serialization (used by FileManager)
    QJsonObject toJson() const;
    static Transaction fromJson(const QJsonObject &obj);

private:
    int m_id;
    TransactionType m_type;
    double m_amount;
    QString m_category;
    QString m_description;
    QDate m_date;
};

#endif // TRANSACTION_H
