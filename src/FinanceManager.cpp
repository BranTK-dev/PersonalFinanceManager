#include "FinanceManager.h"

FinanceManager::FinanceManager()
    : m_nextId(1)
{
}

void FinanceManager::addTransaction(const Transaction &transaction)
{
    Transaction t = transaction;
    if (t.id() < 0) {
        t.setId(nextId());
    }
    m_transactions.append(t);

    if (t.id() >= m_nextId) {
        m_nextId = t.id() + 1;
    }
}

bool FinanceManager::removeTransaction(int id)
{
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].id() == id) {
            m_transactions.removeAt(i);
            return true;
        }
    }
    return false;
}

bool FinanceManager::updateTransaction(const Transaction &updated)
{
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].id() == updated.id()) {
            m_transactions[i] = updated;
            return true;
        }
    }
    return false;
}

const QVector<Transaction>& FinanceManager::transactions() const
{
    return m_transactions;
}

Transaction* FinanceManager::findById(int id)
{
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].id() == id) {
            return &m_transactions[i];
        }
    }
    return nullptr;
}

QVector<Transaction> FinanceManager::search(const QString &keyword) const
{
    QVector<Transaction> results;
    for (const Transaction &t : m_transactions) {
        if (t.description().contains(keyword, Qt::CaseInsensitive) ||
            t.category().contains(keyword, Qt::CaseInsensitive)) {
            results.append(t);
        }
    }
    return results;
}

QVector<Transaction> FinanceManager::filterByType(TransactionType type) const
{
    QVector<Transaction> results;
    for (const Transaction &t : m_transactions) {
        if (t.type() == type) {
            results.append(t);
        }
    }
    return results;
}

QVector<Transaction> FinanceManager::filterByCategory(const QString &category) const
{
    QVector<Transaction> results;
    for (const Transaction &t : m_transactions) {
        if (t.category().compare(category, Qt::CaseInsensitive) == 0) {
            results.append(t);
        }
    }
    return results;
}

double FinanceManager::totalIncome() const
{
    double total = 0.0;
    for (const Transaction &t : m_transactions) {
        if (t.type() == TransactionType::Income) {
            total += t.amount();
        }
    }
    return total;
}

double FinanceManager::totalExpense() const
{
    double total = 0.0;
    for (const Transaction &t : m_transactions) {
        if (t.type() == TransactionType::Expense) {
            total += t.amount();
        }
    }
    return total;
}

double FinanceManager::balance() const
{
    return totalIncome() - totalExpense();
}

int FinanceManager::nextId() const
{
    return m_nextId;
}

void FinanceManager::setTransactions(const QVector<Transaction> &transactions)
{
    m_transactions = transactions;

    // Recalculate next ID so loaded data doesn't collide with new entries
    int maxId = 0;
    for (const Transaction &t : m_transactions) {
        if (t.id() > maxId) {
            maxId = t.id();
        }
    }
    m_nextId = maxId + 1;
}
