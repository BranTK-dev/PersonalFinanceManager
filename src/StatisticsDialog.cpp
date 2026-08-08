#include "StatisticsDialog.h"
#include "ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>
#include <QMap>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

StatisticsDialog::StatisticsDialog(const FinanceManager &financeManager, QWidget *parent)
    : QDialog(parent), m_financeManager(financeManager)
{
    setupUi();
    populateCategoryTable();
    buildExpenseChart();
}

void StatisticsDialog::setupUi()
{
    setWindowTitle("Statistics");
    resize(700, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFont summaryFont;
    summaryFont.setPointSize(12);
    summaryFont.setBold(true);

    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setFont(summaryFont);
    m_summaryLabel->setText(QString("Income: R%1   Expenses: R%2   Balance: R%3")
        .arg(m_financeManager.totalIncome(), 0, 'f', 2)
        .arg(m_financeManager.totalExpense(), 0, 'f', 2)
        .arg(m_financeManager.balance(), 0, 'f', 2));

    m_categoryTable = new QTableWidget(this);
    m_categoryTable->setColumnCount(3);
    m_categoryTable->setHorizontalHeaderLabels({"Category", "Type", "Total"});
    m_categoryTable->horizontalHeader()->setStretchLastSection(true);
    m_categoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(m_summaryLabel);

    // Table on the left, chart on the right, side by side.
    // The chart view gets built and inserted in buildExpenseChart().
    m_contentLayout = new QHBoxLayout();
    m_contentLayout->addWidget(m_categoryTable, 1);
    mainLayout->addLayout(m_contentLayout, 1);
}

void StatisticsDialog::populateCategoryTable()
{
    // category name -> total amount, tracked separately for income and expense
    QMap<QString, double> incomeTotals;
    QMap<QString, double> expenseTotals;

    for (const Transaction &t : m_financeManager.transactions()) {
        if (t.type() == TransactionType::Income) {
            incomeTotals[t.category()] += t.amount();
        } else {
            expenseTotals[t.category()] += t.amount();
        }
    }

    int rowCount = incomeTotals.size() + expenseTotals.size();
    m_categoryTable->setRowCount(rowCount);

    int row = 0;
    for (auto it = incomeTotals.constBegin(); it != incomeTotals.constEnd(); ++it) {
        m_categoryTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        m_categoryTable->setItem(row, 1, new QTableWidgetItem("Income"));
        m_categoryTable->setItem(row, 2, new QTableWidgetItem(QString::number(it.value(), 'f', 2)));
        ++row;
    }
    for (auto it = expenseTotals.constBegin(); it != expenseTotals.constEnd(); ++it) {
        m_categoryTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        m_categoryTable->setItem(row, 1, new QTableWidgetItem("Expense"));
        m_categoryTable->setItem(row, 2, new QTableWidgetItem(QString::number(it.value(), 'f', 2)));
        ++row;
    }
}

void StatisticsDialog::buildExpenseChart()
{
    QMap<QString, double> expenseTotals;
    for (const Transaction &t : m_financeManager.transactions()) {
        if (t.type() == TransactionType::Expense) {
            expenseTotals[t.category()] += t.amount();
        }
    }

    QPieSeries *series = new QPieSeries();
    for (auto it = expenseTotals.constBegin(); it != expenseTotals.constEnd(); ++it) {
        series->append(it.key(), it.value());
    }

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Expenses by Category");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Match whichever theme the app is currently using, otherwise a white chart
    // ends up sitting inside a dark window (or vice versa) and looks broken.
    chart->setTheme(ThemeManager::isDarkModeEnabled() ? QChart::ChartThemeDark : QChart::ChartThemeLight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // If there's no expense data yet, an empty pie chart still renders fine (just shows nothing),
    // no need to special-case it here.

    m_contentLayout->addWidget(chartView, 1);
}
