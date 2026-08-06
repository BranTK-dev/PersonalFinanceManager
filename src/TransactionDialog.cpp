#include "TransactionDialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

TransactionDialog::TransactionDialog(TransactionType type, QWidget *parent)
    : QDialog(parent), m_type(type), m_editingId(-1)
{
    setupUi();
}

TransactionDialog::TransactionDialog(const Transaction &existing, QWidget *parent)
    : QDialog(parent), m_type(existing.type()), m_editingId(existing.id())
{
    setupUi();

    // Pre-fill fields with the existing transaction's values
    int categoryIndex = m_categoryCombo->findText(existing.category());
    if (categoryIndex >= 0) {
        m_categoryCombo->setCurrentIndex(categoryIndex);
    } else {
        m_categoryCombo->setCurrentText(existing.category()); // custom category not in the presets
    }
    m_descriptionEdit->setText(existing.description());
    m_amountSpin->setValue(existing.amount());
    m_dateEdit->setDate(existing.date());
}

void TransactionDialog::setupUi()
{
    setWindowTitle(m_editingId >= 0
        ? (m_type == TransactionType::Income ? "Edit Income" : "Edit Expense")
        : (m_type == TransactionType::Income ? "Add Income" : "Add Expense"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *form = new QFormLayout();

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setEditable(true); // user can type a new category, not just pick presets

    if (m_type == TransactionType::Income) {
        m_categoryCombo->addItems({"Salary", "Freelance", "Investment", "Gift", "Other"});
    } else {
        m_categoryCombo->addItems({"Food", "Rent", "Utilities", "Transport", "Entertainment", "Other"});
    }

    m_descriptionEdit = new QLineEdit(this);

    m_amountSpin = new QDoubleSpinBox(this);
    m_amountSpin->setRange(0.01, 1000000.00);
    m_amountSpin->setDecimals(2);
    m_amountSpin->setPrefix("R ");
    m_amountSpin->setValue(0.01);

    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");

    form->addRow("Category:", m_categoryCombo);
    form->addRow("Description:", m_descriptionEdit);
    form->addRow("Amount:", m_amountSpin);
    form->addRow("Date:", m_dateEdit);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addLayout(form);
    mainLayout->addWidget(m_buttonBox);
}

Transaction TransactionDialog::resultTransaction() const
{
    Transaction t;
    t.setId(m_editingId); // -1 for a new transaction, or the original id when editing
    t.setType(m_type);
    t.setAmount(m_amountSpin->value());
    t.setCategory(m_categoryCombo->currentText());
    t.setDescription(m_descriptionEdit->text());
    t.setDate(m_dateEdit->date());
    return t;
}
