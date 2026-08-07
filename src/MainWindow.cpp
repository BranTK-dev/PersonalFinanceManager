#include "MainWindow.h"
#include "TransactionDialog.h"
#include "FileManager.h"
#include "StatisticsDialog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QStatusBar>

// Column 0's item stores the transaction id in Qt::UserRole so edit/delete
// know which transaction a row refers to, even while a search/filter is active.
static const int TransactionIdRole = Qt::UserRole;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    loadData();
    updateTotals();
    applyFilters();
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

    m_balanceLabel = new QLabel("Balance: R0.00", central);
    m_incomeLabel = new QLabel("Income: R0.00", central);
    m_expenseLabel = new QLabel("Expenses: R0.00", central);

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
    m_editButton = new QPushButton("Edit", central);
    m_deleteButton = new QPushButton("Delete", central);
    m_statisticsButton = new QPushButton("Statistics", central);

    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);

    buttonLayout->addWidget(m_addIncomeButton);
    buttonLayout->addWidget(m_addExpenseButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_statisticsButton);
    buttonLayout->addStretch();

    connect(m_addIncomeButton, &QPushButton::clicked, this, &MainWindow::onAddIncomeClicked);
    connect(m_addExpenseButton, &QPushButton::clicked, this, &MainWindow::onAddExpenseClicked);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(m_statisticsButton, &QPushButton::clicked, this, &MainWindow::onStatisticsClicked);

    // --- Search / filter row ---
    QHBoxLayout *searchLayout = new QHBoxLayout();

    m_searchEdit = new QLineEdit(central);
    m_searchEdit->setPlaceholderText("Search by description or category...");

    m_filterCombo = new QComboBox(central);
    m_filterCombo->addItems({"All", "Income", "Expense"});

    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_filterCombo);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(m_filterCombo, &QComboBox::currentIndexChanged, this, &MainWindow::onFilterChanged);

    // --- Transaction table ---
    m_transactionTable = new QTableWidget(central);
    m_transactionTable->setColumnCount(5);
    m_transactionTable->setHorizontalHeaderLabels({"Date", "Type", "Category", "Description", "Amount"});
    m_transactionTable->horizontalHeader()->setStretchLastSection(true);
    m_transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_transactionTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onSelectionChanged);

    mainLayout->addLayout(summaryLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
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
        updateTotals();
        applyFilters();
        saveData();
    }
}

void MainWindow::onEditClicked()
{
    int id = selectedTransactionId();
    if (id < 0) {
        return;
    }

    Transaction *existing = m_financeManager.findById(id);
    if (!existing) {
        return;
    }

    TransactionDialog dialog(*existing, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_financeManager.updateTransaction(dialog.resultTransaction());
        updateTotals();
        applyFilters();
        saveData();
    }
}

void MainWindow::onDeleteClicked()
{
    int id = selectedTransactionId();
    if (id < 0) {
        return;
    }

    Transaction *existing = m_financeManager.findById(id);
    if (!existing) {
        return;
    }

    QString label = QString("%1 - %2 (R%3)")
                         .arg(existing->category())
                         .arg(existing->description())
                         .arg(existing->amount(), 0, 'f', 2);

    auto reply = QMessageBox::question(this, "Delete Transaction",
        QString("Delete this transaction?\n\n%1").arg(label),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_financeManager.removeTransaction(id);
        updateTotals();
        applyFilters();
        saveData();
    }
}

void MainWindow::onSelectionChanged()
{
    bool hasSelection = !m_transactionTable->selectedItems().isEmpty();
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    applyFilters();
}

void MainWindow::onFilterChanged(int index)
{
    Q_UNUSED(index);
    applyFilters();
}

int MainWindow::selectedTransactionId() const
{
    QList<QTableWidgetItem *> selected = m_transactionTable->selectedItems();
    if (selected.isEmpty()) {
        return -1;
    }

    int row = selected.first()->row();
    QTableWidgetItem *idItem = m_transactionTable->item(row, 0);
    if (!idItem) {
        return -1;
    }

    return idItem->data(TransactionIdRole).toInt();
}

void MainWindow::updateTotals()
{
    // Totals always reflect every transaction, regardless of the current search/filter.
    m_balanceLabel->setText(QString("Balance: R%1").arg(m_financeManager.balance(), 0, 'f', 2));
    m_incomeLabel->setText(QString("Income: R%1").arg(m_financeManager.totalIncome(), 0, 'f', 2));
    m_expenseLabel->setText(QString("Expenses: R%1").arg(m_financeManager.totalExpense(), 0, 'f', 2));
}

void MainWindow::applyFilters()
{
    QString keyword = m_searchEdit->text();
    int filterIndex = m_filterCombo->currentIndex(); // 0 = All, 1 = Income, 2 = Expense

    QVector<Transaction> base = m_financeManager.transactions();

    // Apply type filter first
    QVector<Transaction> typeFiltered;
    if (filterIndex == 1) {
        typeFiltered = m_financeManager.filterByType(TransactionType::Income);
    } else if (filterIndex == 2) {
        typeFiltered = m_financeManager.filterByType(TransactionType::Expense);
    } else {
        typeFiltered = base;
    }

    // Then apply the search keyword on top, unless it's empty
    if (keyword.trimmed().isEmpty()) {
        populateTable(typeFiltered);
        return;
    }

    QVector<Transaction> results;
    for (const Transaction &t : typeFiltered) {
        if (t.description().contains(keyword, Qt::CaseInsensitive) ||
            t.category().contains(keyword, Qt::CaseInsensitive)) {
            results.append(t);
        }
    }
    populateTable(results);
}

void MainWindow::populateTable(const QVector<Transaction> &list)
{
    m_transactionTable->setRowCount(list.size());
    for (int row = 0; row < list.size(); ++row) {
        const Transaction &t = list[row];

        QTableWidgetItem *dateItem = new QTableWidgetItem(t.date().toString(Qt::ISODate));
        dateItem->setData(TransactionIdRole, t.id());

        m_transactionTable->setItem(row, 0, dateItem);
        m_transactionTable->setItem(row, 1, new QTableWidgetItem(t.typeToString()));
        m_transactionTable->setItem(row, 2, new QTableWidgetItem(t.category()));
        m_transactionTable->setItem(row, 3, new QTableWidgetItem(t.description()));
        m_transactionTable->setItem(row, 4, new QTableWidgetItem(QString::number(t.amount(), 'f', 2)));
    }

    // Selection was cleared by the repopulate, so disable Edit/Delete until something is picked again.
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
}

void MainWindow::onStatisticsClicked()
{
    StatisticsDialog dialog(m_financeManager, this);
    dialog.exec();
}

void MainWindow::loadData()
{
    QVector<Transaction> loaded;
    if (FileManager::loadFromFile(FileManager::defaultFilePath(), loaded)) {
        m_financeManager.setTransactions(loaded);
        statusBar()->showMessage(QString("Loaded %1 transaction(s) from disk").arg(loaded.size()), 3000);
    } else {
        // No file yet (first run) or it couldn't be read, either way, start with an empty ledger.
        statusBar()->showMessage("Starting with no saved data", 3000);
    }
}

void MainWindow::saveData()
{
    if (!FileManager::saveToFile(FileManager::defaultFilePath(), m_financeManager.transactions())) {
        statusBar()->showMessage("Warning: could not save data to disk", 5000);
    }
}
