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

    // Edit mode: pre-fills fields from an existing transaction and preserves its id on save
    explicit TransactionDialog(const Transaction &existing, QWidget *parent = nullptr);

    // Builds a Transaction from the current field values.
    // In add mode, id is left at -1 so FinanceManager assigns the real id.
    // In edit mode, the original id is preserved so FinanceManager can update in place.
    Transaction resultTransaction() const;

private:
    void setupUi();

    TransactionType m_type;
    int m_editingId; // -1 when adding a new transaction, otherwise the id being edited

    QComboBox *m_categoryCombo;
    QLineEdit *m_descriptionEdit;
    QDoubleSpinBox *m_amountSpin;
    QDateEdit *m_dateEdit;
    QDialogButtonBox *m_buttonBox;
};

#endif // TRANSACTIONDIALOG_H
