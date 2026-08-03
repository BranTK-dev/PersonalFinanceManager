#include "MainWindow.h"
#include "TransactionDialog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    refreshDashboard();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    setWindowTitle("Personal Finance Manager");
    resize(900, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // --- Dashboard summary row ---
    QHBoxLayout *summaryLayout = new QHBoxLayout();

    QFont summaryFont;
    summaryFont.setPointSize(14);
    summaryFont.setBold(true);

    m_balanceLabel = new QLabel("Balance: $0.00", central);
    m_incomeLabel = new QLabel("Income: $0.00", central);
    m_expenseLabel = new QLabel("Expenses: $0.00", central);

    m_balanceLabel->setFont(summaryFont);
    m_incomeLabel->setFont(summaryFont);
    m_expenseLabel->setFont(summaryFont);

    summaryLayout->addWidget(m_balanceLabel);
    summaryLayout->addWidget(m_incomeLabel);
    summaryLayout->addWidget(m_expenseLabel);
    summaryLayout->addStretch();

    // --- Action buttons row ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addIncomeButton = new QPushButton("Add Income", central);
    m_addExpenseButton = new QPushButton("Add Expense", central);

    buttonLayout->addWidget(m_addIncomeButton);
    buttonLayout->addWidget(m_addExpenseButton);
    buttonLayout->addStretch();

    connect(m_addIncomeButton, &QPushButton::clicked, this, &MainWindow::onAddIncomeClicked);
    connect(m_addExpenseButton, &QPushButton::clicked, this, &MainWindow::onAddExpenseClicked);

    // --- Transaction table ---
    m_transactionTable = new QTableWidget(central);
    m_transactionTable->setColumnCount(5);
    m_transactionTable->setHorizontalHeaderLabels({"Date", "Type", "Category", "Description", "Amount"});
    m_transactionTable->horizontalHeader()->setStretchLastSection(true);
    m_transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    mainLayout->addLayout(summaryLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_transactionTable);
}

void MainWindow::onAddIncomeClicked()
{
    openAddDialog(TransactionType::Income);
}

void MainWindow::onAddExpenseClicked()
{
    openAddDialog(TransactionType::Expense);
}

void MainWindow::openAddDialog(TransactionType type)
{
    TransactionDialog dialog(type, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_financeManager.addTransaction(dialog.resultTransaction());
        refreshDashboard();
    }
}

void MainWindow::refreshDashboard()
{
    m_balanceLabel->setText(QString("Balance: $%1").arg(m_financeManager.balance(), 0, 'f', 2));
    m_incomeLabel->setText(QString("Income: $%1").arg(m_financeManager.totalIncome(), 0, 'f', 2));
    m_expenseLabel->setText(QString("Expenses: $%1").arg(m_financeManager.totalExpense(), 0, 'f', 2));

    // Refresh table
    const QVector<Transaction> &transactions = m_financeManager.transactions();
    m_transactionTable->setRowCount(transactions.size());
    for (int row = 0; row < transactions.size(); ++row) {
        const Transaction &t = transactions[row];
        m_transactionTable->setItem(row, 0, new QTableWidgetItem(t.date().toString(Qt::ISODate)));
        m_transactionTable->setItem(row, 1, new QTableWidgetItem(t.typeToString()));
        m_transactionTable->setItem(row, 2, new QTableWidgetItem(t.category()));
        m_transactionTable->setItem(row, 3, new QTableWidgetItem(t.description()));
        m_transactionTable->setItem(row, 4, new QTableWidgetItem(QString::number(t.amount(), 'f', 2)));
    }
}
