#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include "FinanceManager.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QLabel;
class QHBoxLayout;
QT_END_NAMESPACE

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    // Takes a const reference since this dialog only reads data, it never modifies transactions.
    explicit StatisticsDialog(const FinanceManager &financeManager, QWidget *parent = nullptr);

private:
    void setupUi();
    void populateCategoryTable();
    void buildExpenseChart();

    const FinanceManager &m_financeManager;

    QLabel *m_summaryLabel;
    QTableWidget *m_categoryTable;
    QHBoxLayout *m_contentLayout;
};

#endif // STATISTICSDIALOG_H
