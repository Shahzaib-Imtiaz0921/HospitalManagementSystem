#include "doctorspage.h"
#include "../manager/systemmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QHeaderView>

DoctorsPage::DoctorsPage(SystemManager* sm, QWidget* parent)
    : QWidget(parent), sysManager(sm)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel* title = new QLabel("👨‍⚕️ Doctors", this);
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #1a2744;");
    layout->addWidget(title);

    // Subtitle
    QLabel* subtitle = new QLabel("All doctors across all hospitals", this);
    QFont subtitleFont;
    subtitleFont.setPointSize(12);
    subtitle->setFont(subtitleFont);
    subtitle->setStyleSheet("color: #7f8c8d;");
    layout->addWidget(subtitle);

    // Filter row
    QHBoxLayout* filterLayout = new QHBoxLayout();
    QLabel* filterLabel = new QLabel("Filter by Hospital:", this);
    filterCombo = new QComboBox(this);
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterCombo);
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Doctor ID", "Name", "Specialization", "Hospital", "Availability"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    layout->addWidget(table, 1);

    connect(filterCombo, &QComboBox::currentTextChanged,
            this, &DoctorsPage::onFilterChanged);

    refresh();
}

void DoctorsPage::refresh()
{
    QList<QMap<QString, QString>> allData = sysManager->getAllDoctorsData();

    // Block signals while repopulating the combo box
    filterCombo->blockSignals(true);
    filterCombo->clear();
    filterCombo->addItem("All Hospitals");

    QStringList seen;
    for (const QMap<QString, QString>& row : allData) {
        const QString& hosp = row["hospital_name"];
        if (!hosp.isEmpty() && !seen.contains(hosp)) {
            seen.append(hosp);
            filterCombo->addItem(hosp);
        }
    }
    filterCombo->blockSignals(false);

    populateTable(allData);
}

void DoctorsPage::populateTable(const QList<QMap<QString, QString>>& data)
{
    table->setRowCount(data.size());

    for (int row = 0; row < data.size(); ++row) {
        const QMap<QString, QString>& d = data[row];
        table->setItem(row, 0, new QTableWidgetItem(d["did"]));
        table->setItem(row, 1, new QTableWidgetItem(d["name"]));
        table->setItem(row, 2, new QTableWidgetItem(d["spec"]));
        table->setItem(row, 3, new QTableWidgetItem(d["hospital_name"]));

        QTableWidgetItem* availItem;
        if (d["available"] == "1") {
            availItem = new QTableWidgetItem("Available");
            availItem->setForeground(QColor(39, 174, 96));
        } else {
            availItem = new QTableWidgetItem("Busy");
            availItem->setForeground(QColor(192, 57, 43));
        }
        table->setItem(row, 4, availItem);
    }

    table->resizeColumnsToContents();
}

void DoctorsPage::onFilterChanged(const QString& hospitalName)
{
    if (hospitalName == "All Hospitals") {
        populateTable(sysManager->getAllDoctorsData());
    } else {
        QList<QMap<QString, QString>> allData = sysManager->getAllDoctorsData();
        QList<QMap<QString, QString>> filtered;
        for (const QMap<QString, QString>& row : allData) {
            if (row["hospital_name"] == hospitalName) {
                filtered.append(row);
            }
        }
        populateTable(filtered);
    }
}
