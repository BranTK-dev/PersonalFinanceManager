#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Transaction.h"
#include "FinanceManager.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
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
    void onEditClicked();
    void onDeleteClicked();
    void onSelectionChanged();
    void onSearchTextChanged(const QString &text);
    void onFilterChanged(int index);
    void onStatisticsClicked();
    void onToggleDarkMode(bool checked);

private:
    void setupUi();
    void openAddDialog(TransactionType type);
    void loadData();
    void saveData();

    // Splitting the old refreshDashboard() into two steps:
    // totals always reflect ALL transactions, the table reflects the current search/filter.
    void updateTotals();
    void applyFilters();
    void populateTable(const QVector<Transaction> &list);

    // Returns the id of the transaction on the currently selected row, or -1 if none selected.
    int selectedTransactionId() const;

    // Widgets - summary
    QLabel *m_balanceLabel;
    QLabel *m_incomeLabel;
    QLabel *m_expenseLabel;

    // Widgets - actions
    QPushButton *m_addIncomeButton;
    QPushButton *m_addExpenseButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QPushButton *m_statisticsButton;

    // Widgets - search/filter
    QLineEdit *m_searchEdit;
    QComboBox *m_filterCombo;

    // Widgets - table
    QTableWidget *m_transactionTable;

    // Business logic
    FinanceManager m_financeManager;
};

#endif // MAINWINDOW_H
