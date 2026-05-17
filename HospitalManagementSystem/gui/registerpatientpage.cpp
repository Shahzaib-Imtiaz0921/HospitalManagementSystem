#include "registerpatientpage.h"
#include "consultationresultdialog.h"
#include "../manager/systemmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QTableWidgetItem>
#include <QString>

RegisterPatientPage::RegisterPatientPage(SystemManager* sm, QWidget* parent)
    : QWidget(parent)
    , sysManager(sm)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // Page title
    QLabel* titleLabel = new QLabel("📋 Register Patient", this);
    QFont titleFont;
    titleFont.setPixelSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1a2744;");
    mainLayout->addWidget(titleLabel);

    // Subtitle
    QLabel* subtitleLabel = new QLabel("Fill in patient details and find the nearest hospital", this);
    subtitleLabel->setStyleSheet("color: #7f8c8d; font-size: 13px;");
    mainLayout->addWidget(subtitleLabel);

    // Two-column layout: left form, right table
    QHBoxLayout* columnsLayout = new QHBoxLayout();
    columnsLayout->setSpacing(20);

    // Left panel (form)
    QWidget* formWidget = new QWidget(this);
    setupForm(formWidget);
    columnsLayout->addWidget(formWidget, 1);

    // Right panel (hospital table)
    QWidget* tableWidget = new QWidget(this);
    setupHospitalTable(tableWidget);
    columnsLayout->addWidget(tableWidget, 2);

    mainLayout->addLayout(columnsLayout);

    populateSectors();
}

void RegisterPatientPage::setupForm(QWidget* formWidget)
{
    QVBoxLayout* layout = new QVBoxLayout(formWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QLabel* formTitle = new QLabel("Patient Information", formWidget);
    QFont ft;
    ft.setBold(true);
    ft.setPixelSize(14);
    formTitle->setFont(ft);
    formTitle->setStyleSheet("color: #1a2744;");
    layout->addWidget(formTitle);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    nameEdit = new QLineEdit(formWidget);
    nameEdit->setPlaceholderText("Enter patient name");
    formLayout->addRow("Patient Name:", nameEdit);

    ageSpin = new QSpinBox(formWidget);
    ageSpin->setRange(1, 120);
    ageSpin->setValue(25);
    formLayout->addRow("Age:", ageSpin);

    locationCombo = new QComboBox(formWidget);
    formLayout->addRow("Location (Sector):", locationCombo);

    symptomsEdit = new QTextEdit(formWidget);
    symptomsEdit->setPlaceholderText("Describe symptoms...");
    symptomsEdit->setMaximumHeight(100);
    formLayout->addRow("Symptoms:", symptomsEdit);

    layout->addLayout(formLayout);

    findBtn = new QPushButton("🔍 Find Nearest Hospitals", formWidget);
    layout->addWidget(findBtn);

    layout->addStretch();

    connect(findBtn, &QPushButton::clicked, this, &RegisterPatientPage::onFindHospitals);
}

void RegisterPatientPage::setupHospitalTable(QWidget* tableWidget)
{
    QVBoxLayout* layout = new QVBoxLayout(tableWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    recommendedLabel = new QLabel("Select a hospital from the list below", tableWidget);
    recommendedLabel->setStyleSheet("color: #7f8c8d; font-style: italic;");
    layout->addWidget(recommendedLabel);

    hospitalTable = new QTableWidget(tableWidget);
    hospitalTable->setColumnCount(4);
    hospitalTable->setHorizontalHeaderLabels({"Rank", "Hospital Name", "Match Score", "Match Quality"});
    hospitalTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hospitalTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    hospitalTable->horizontalHeader()->setStretchLastSection(true);
    hospitalTable->setAlternatingRowColors(true);
    layout->addWidget(hospitalTable);

    connect(hospitalTable, &QTableWidget::cellClicked,
            this, [this](int row, int /*col*/) {
                QTableWidgetItem* item = hospitalTable->item(row, 0);
                if (item) {
                    selectedHospitalIdx = item->data(Qt::UserRole).toInt();
                }
            });

    registerBtn = new QPushButton("✅ Register Patient & Proceed", tableWidget);
    registerBtn->setEnabled(false);
    registerBtn->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 10px 20px; border-radius: 4px;");
    layout->addWidget(registerBtn);

    connect(registerBtn, &QPushButton::clicked, this, &RegisterPatientPage::onRegisterAndProceed);
}

void RegisterPatientPage::populateSectors()
{
    locationCombo->clear();
    const QStringList sectors = {
        "G-6", "G-7", "G-8", "G-9", "G-10", "G-11", "G-12", "G-13",
        "F-6", "F-7", "F-8", "F-9", "F-10", "F-11", "F-12",
        "H-8", "H-9", "H-10", "H-11", "H-12", "H-13",
        "Blue Area", "Saddar", "I-8"
    };
    locationCombo->addItems(sectors);
}

void RegisterPatientPage::refresh()
{
    // Nothing to reload on this page; sectors are static
}

void RegisterPatientPage::onFindHospitals()
{
    // Validate inputs
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter the patient name.");
        return;
    }
    if (symptomsEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please describe the patient's symptoms.");
        return;
    }

    QString sector = locationCombo->currentText();
    QVector<QPair<int,int>> ranked = sysManager->rankHospitals(sector);

    if (ranked.isEmpty()) {
        QMessageBox::information(this, "No Results", "No hospitals found for the selected sector.");
        return;
    }

    hospitalTable->setRowCount(ranked.size());

    const auto& hospitals = sysManager->getHospitals();

    for (int i = 0; i < ranked.size(); ++i) {
        int score      = ranked[i].first;
        int hospIdx    = ranked[i].second;

        QString hospName = (hospIdx >= 0 && hospIdx < (int)hospitals.size())
                           ? QString::fromStdString(hospitals[hospIdx].getName())
                           : QString("Unknown");

        QString scoreStr = QString("%1%").arg(score);

        QString quality;
        if (score >= 90)      quality = "Excellent 🟢";
        else if (score >= 70) quality = "Good 🟡";
        else if (score >= 50) quality = "Moderate 🟠";
        else                  quality = "Far 🔴";

        QTableWidgetItem* rankItem  = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem* nameItem  = new QTableWidgetItem(hospName);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(scoreStr);
        QTableWidgetItem* qualItem  = new QTableWidgetItem(quality);

        // Store actual hospital index in UserRole of rank item
        rankItem->setData(Qt::UserRole, hospIdx);

        // Center-align rank and score
        rankItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);

        hospitalTable->setItem(i, 0, rankItem);
        hospitalTable->setItem(i, 1, nameItem);
        hospitalTable->setItem(i, 2, scoreItem);
        hospitalTable->setItem(i, 3, qualItem);

        // Highlight top row (recommended) with light blue
        if (i == 0) {
            QColor highlight(232, 244, 253);
            rankItem->setBackground(QBrush(highlight));
            nameItem->setBackground(QBrush(highlight));
            scoreItem->setBackground(QBrush(highlight));
            qualItem->setBackground(QBrush(highlight));
        }
    }

    // Pre-select top hospital
    selectedHospitalIdx = ranked[0].second;
    hospitalTable->selectRow(0);

    // Update recommended label
    const QString topName = (selectedHospitalIdx >= 0 && selectedHospitalIdx < (int)hospitals.size())
                            ? QString::fromStdString(hospitals[selectedHospitalIdx].getName())
                            : "Unknown";
    recommendedLabel->setText(
        QString("✅ Recommended: %1 (%2% match)").arg(topName).arg(ranked[0].first)
    );
    recommendedLabel->setStyleSheet("color: #27ae60; font-weight: bold;");

    registerBtn->setEnabled(true);
}

void RegisterPatientPage::onRegisterAndProceed()
{
    // Validate all fields
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter the patient name.");
        return;
    }
    QString symptoms = symptomsEdit->toPlainText().trimmed();
    if (symptoms.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please describe the patient's symptoms.");
        return;
    }

    if (selectedHospitalIdx < 0) {
        QMessageBox::warning(this, "No Hospital Selected", "Please find and select a hospital first.");
        return;
    }

    // Determine actual hospital index from selected row (UserRole data)
    int hospitalIdx = selectedHospitalIdx;
    int currentRow = hospitalTable->currentRow();
    if (currentRow >= 0) {
        QTableWidgetItem* item = hospitalTable->item(currentRow, 0);
        if (item) {
            hospitalIdx = item->data(Qt::UserRole).toInt();
        }
    }

    int age = ageSpin->value();
    QString sector = locationCombo->currentText();

    // Register patient
    Patient patient = sysManager->registerPatient(name, age, sector, symptoms, hospitalIdx);

    // Assign doctor
    auto& hospitals = sysManager->getHospitals();
    if (hospitalIdx < 0 || hospitalIdx >= (int)hospitals.size()) {
        QMessageBox::warning(this, "Error", "Invalid hospital selection.");
        return;
    }

    Doctor* doctor = sysManager->assignDoctor(patient, hospitals[hospitalIdx]);
    if (doctor == nullptr) {
        QMessageBox::warning(this, "No Doctor Available",
                             "No available doctor at this hospital. Please try another.");
        return;
    }

    // Conduct consultation
    ConsultationSession session = sysManager->conductConsultation(patient, *doctor);

    // Show result dialog
    ConsultationResultDialog dlg(session, patient, *doctor, sysManager, this);
    dlg.exec();
    emit patientRegistered();

    // Clear form after dialog closes
    nameEdit->clear();
    ageSpin->setValue(1);
    locationCombo->setCurrentIndex(0);
    symptomsEdit->clear();
    hospitalTable->setRowCount(0);
    registerBtn->setEnabled(false);
    recommendedLabel->setText("Select a hospital from the list below");
    recommendedLabel->setStyleSheet("color: #7f8c8d; font-style: italic;");
    selectedHospitalIdx = -1;
}
