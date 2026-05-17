#include "hospitalspage.h"
#include "../manager/systemmanager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QHeaderView>

HospitalsPage::HospitalsPage(SystemManager* sm, QWidget* parent)
    : QWidget(parent), sysManager(sm)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel* title = new QLabel("🏨 Hospitals", this);
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #1a2744;");
    layout->addWidget(title);

    // Subtitle
    QLabel* subtitle = new QLabel("All registered hospitals in the system", this);
    QFont subtitleFont;
    subtitleFont.setPointSize(12);
    subtitle->setFont(subtitleFont);
    subtitle->setStyleSheet("color: #7f8c8d;");
    layout->addWidget(subtitle);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Hospital ID", "Name", "Location", "Capacity", "Specialty", "Doctors"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    layout->addWidget(table, 1);

    refresh();
}

void HospitalsPage::refresh()
{
    QList<QMap<QString, QString>> data = sysManager->getAllHospitalsData();
    table->setRowCount(data.size());

    for (int row = 0; row < data.size(); ++row) {
        const QMap<QString, QString>& h = data[row];
        table->setItem(row, 0, new QTableWidgetItem(h["hid"]));
        table->setItem(row, 1, new QTableWidgetItem(h["name"]));
        table->setItem(row, 2, new QTableWidgetItem(h["loc"]));
        table->setItem(row, 3, new QTableWidgetItem(h["cap"]));
        table->setItem(row, 4, new QTableWidgetItem(h["spec"]));
        table->setItem(row, 5, new QTableWidgetItem(h["doc_count"]));
    }

    table->resizeColumnsToContents();
}
