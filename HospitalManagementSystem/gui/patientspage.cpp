#include "patientspage.h"
#include "../manager/systemmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColor>
#include <QBrush>

PatientsPage::PatientsPage(SystemManager* sm, QWidget* parent)
    : QWidget(parent), sysManager(sm)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    // Title
    QLabel* title = new QLabel("👥 Patients", this);
    QFont titleFont; titleFont.setPixelSize(20); titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #1a2744;");
    layout->addWidget(title);

    // Subtitle
    QLabel* subtitle = new QLabel("All registered patients and their current status", this);
    subtitle->setStyleSheet("color: #7f8c8d; font-size: 13px;");
    layout->addWidget(subtitle);

    // Toolbar
    QHBoxLayout* toolbar = new QHBoxLayout();
    refreshBtn = new QPushButton("🔄 Refresh", this);
    exportBtn  = new QPushButton("📥 Export to CSV", this);
    toolbar->addWidget(refreshBtn);
    toolbar->addWidget(exportBtn);
    toolbar->addStretch();
    layout->addLayout(toolbar);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"Patient ID","Name","Age","Location","Symptoms","Hospital","Doctor","Status"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->setWordWrap(true);
    layout->addWidget(table, 1);

    connect(refreshBtn, &QPushButton::clicked, this, &PatientsPage::refresh);
    connect(exportBtn,  &QPushButton::clicked, this, &PatientsPage::onExportCSV);

    refresh();
}

void PatientsPage::refresh()
{
    QList<QMap<QString,QString>> data = sysManager->getAllPatientsData();
    table->setRowCount(data.size());

    for (int row = 0; row < data.size(); ++row) {
        const QMap<QString,QString>& p = data[row];
        table->setItem(row, 0, new QTableWidgetItem(p["pid"]));
        table->setItem(row, 1, new QTableWidgetItem(p["name"]));
        table->setItem(row, 2, new QTableWidgetItem(p["age"]));
        table->setItem(row, 3, new QTableWidgetItem(p["location"]));
        table->setItem(row, 4, new QTableWidgetItem(p["symptoms"]));
        table->setItem(row, 5, new QTableWidgetItem(p["hospital_name"]));
        table->setItem(row, 6, new QTableWidgetItem(p["doctor_name"]));

        QTableWidgetItem* statusItem = new QTableWidgetItem(p["status"]);
        QFont boldFont; boldFont.setBold(true);
        statusItem->setFont(boldFont);
        if (p["status"] == "Active") {
            statusItem->setForeground(QColor(46, 134, 222));
        } else if (p["status"] == "Discharged") {
            statusItem->setForeground(QColor(39, 174, 96));
        }
        table->setItem(row, 7, statusItem);
    }

    table->resizeColumnsToContents();
}

void PatientsPage::onExportCSV()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Patients", "patients.csv", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    bool ok = sysManager->getDB().exportPatientsCSV(filePath);
    if (ok) {
        QMessageBox::information(this, "Export Successful",
            "Patients exported to:\n" + filePath);
    } else {
        QMessageBox::warning(this, "Export Failed",
            "Could not write to file:\n" + filePath);
    }
}
