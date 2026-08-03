#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include "Transaction.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QDialogButtonBox;
QT_END_NAMESPACE

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    // type determines the dialog title and default category list shown to the user
    explicit TransactionDialog(TransactionType type, QWidget *parent = nullptr);

    // Builds a Transaction from the current field values.
    // id is left at -1; FinanceManager assigns the real id on add.
    Transaction resultTransaction() const;

private:
    void setupUi();

    TransactionType m_type;

    QComboBox *m_categoryCombo;
    QLineEdit *m_descriptionEdit;
    QDoubleSpinBox *m_amountSpin;
    QDateEdit *m_dateEdit;
    QDialogButtonBox *m_buttonBox;
};

#endif // TRANSACTIONDIALOG_H
