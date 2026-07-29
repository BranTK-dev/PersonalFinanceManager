#include "Transaction.h"

Transaction::Transaction()
    : m_id(-1), m_type(TransactionType::Expense), m_amount(0.0),
      m_category(""), m_description(""), m_date(QDate::currentDate())
{
}

Transaction::Transaction(int id, TransactionType type, double amount, const QString &category,
                          const QString &description, const QDate &date)
    : m_id(id), m_type(type), m_amount(amount),
      m_category(category), m_description(description), m_date(date)
{
}

int Transaction::id() const { return m_id; }
TransactionType Transaction::type() const { return m_type; }
double Transaction::amount() const { return m_amount; }
QString Transaction::category() const { return m_category; }
QString Transaction::description() const { return m_description; }
QDate Transaction::date() const { return m_date; }

void Transaction::setId(int id) { m_id = id; }
void Transaction::setType(TransactionType type) { m_type = type; }
void Transaction::setAmount(double amount) { m_amount = amount; }
void Transaction::setCategory(const QString &category) { m_category = category; }
void Transaction::setDescription(const QString &description) { m_description = description; }
void Transaction::setDate(const QDate &date) { m_date = date; }

QString Transaction::typeToString() const
{
    return m_type == TransactionType::Income ? "Income" : "Expense";
}

TransactionType Transaction::stringToType(const QString &typeStr)
{
    return typeStr == "Income" ? TransactionType::Income : TransactionType::Expense;
}

QJsonObject Transaction::toJson() const
{
    QJsonObject obj;
    obj["id"] = m_id;
    obj["type"] = typeToString();
    obj["amount"] = m_amount;
    obj["category"] = m_category;
    obj["description"] = m_description;
    obj["date"] = m_date.toString(Qt::ISODate);
    return obj;
}

Transaction Transaction::fromJson(const QJsonObject &obj)
{
    Transaction t;
    t.setId(obj["id"].toInt());
    t.setType(stringToType(obj["type"].toString()));
    t.setAmount(obj["amount"].toDouble());
    t.setCategory(obj["category"].toString());
    t.setDescription(obj["description"].toString());
    t.setDate(QDate::fromString(obj["date"].toString(), Qt::ISODate));
    return t;
}
