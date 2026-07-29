#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Transaction.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddIncomeClicked();
    void onAddExpenseClicked();

private:
    void setupUi();
    void refreshDashboard();

    // Widgets
    QLabel *m_balanceLabel;
    QLabel *m_incomeLabel;
    QLabel *m_expenseLabel;
    QTableWidget *m_transactionTable;
    QPushButton *m_addIncomeButton;
    QPushButton *m_addExpenseButton;

    // Data (temporary in-memory store until FinanceManager is wired in)
    QVector<Transaction> m_transactions;
};

#endif // MAINWINDOW_H
