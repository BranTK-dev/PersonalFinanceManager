#include "MainWindow.h"

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
    // Placeholder: this will open an "Add Income" dialog in Phase 2
    // and push a new Transaction into m_transactions, then call refreshDashboard().
}

void MainWindow::onAddExpenseClicked()
{
    // Placeholder: this will open an "Add Expense" dialog in Phase 2
    // and push a new Transaction into m_transactions, then call refreshDashboard().
}

void MainWindow::refreshDashboard()
{
    double income = 0.0;
    double expense = 0.0;

    for (const Transaction &t : m_transactions) {
        if (t.type() == TransactionType::Income)
            income += t.amount();
        else
            expense += t.amount();
    }

    double balance = income - expense;

    m_balanceLabel->setText(QString("Balance: $%1").arg(balance, 0, 'f', 2));
    m_incomeLabel->setText(QString("Income: $%1").arg(income, 0, 'f', 2));
    m_expenseLabel->setText(QString("Expenses: $%1").arg(expense, 0, 'f', 2));

    // Refresh table
    m_transactionTable->setRowCount(m_transactions.size());
    for (int row = 0; row < m_transactions.size(); ++row) {
        const Transaction &t = m_transactions[row];
        m_transactionTable->setItem(row, 0, new QTableWidgetItem(t.date().toString(Qt::ISODate)));
        m_transactionTable->setItem(row, 1, new QTableWidgetItem(t.typeToString()));
        m_transactionTable->setItem(row, 2, new QTableWidgetItem(t.category()));
        m_transactionTable->setItem(row, 3, new QTableWidgetItem(t.description()));
        m_transactionTable->setItem(row, 4, new QTableWidgetItem(QString::number(t.amount(), 'f', 2)));
    }
}
