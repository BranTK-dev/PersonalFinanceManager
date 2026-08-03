#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Transaction.h"
#include "FinanceManager.h"

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
    void openAddDialog(TransactionType type);

    // Widgets
    QLabel *m_balanceLabel;
    QLabel *m_incomeLabel;
    QLabel *m_expenseLabel;
    QTableWidget *m_transactionTable;
    QPushButton *m_addIncomeButton;
    QPushButton *m_addExpenseButton;

    // Business logic
    FinanceManager m_financeManager;
};

#endif // MAINWINDOW_H
