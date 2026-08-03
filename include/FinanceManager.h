#ifndef FINANCEMANAGER_H
#define FINANCEMANAGER_H

#include <QVector>
#include "Transaction.h"

class FinanceManager
{
public:
    FinanceManager();

    // Core operations
    void addTransaction(const Transaction &transaction);
    bool removeTransaction(int id);
    bool updateTransaction(const Transaction &updated);

    // Access
    const QVector<Transaction>& transactions() const;
    Transaction* findById(int id);

    // Search / filter
    QVector<Transaction> search(const QString &keyword) const;
    QVector<Transaction> filterByType(TransactionType type) const;
    QVector<Transaction> filterByCategory(const QString &category) const;

    // Statistics
    double totalIncome() const;
    double totalExpense() const;
    double balance() const;

    // ID management
    int nextId() const;

    // Used by FileManager when loading from disk
    void setTransactions(const QVector<Transaction> &transactions);

private:
    QVector<Transaction> m_transactions;
    int m_nextId;
};

#endif // FINANCEMANAGER_H
