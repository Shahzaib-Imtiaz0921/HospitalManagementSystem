#include "dashboardpage.h"
#include "../manager/systemmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>

DashboardPage::DashboardPage(SystemManager* sm, QWidget* parent)
    : QWidget(parent), sysManager(sm)
{
    setStyleSheet("background-color: #f0f4f8;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // --- Header section ---
    QLabel* titleLabel = new QLabel("🏥 Hospital Management System", this);
    QFont titleFont;
    titleFont.setPixelSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #1a2744;");
    mainLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("Bahria University — OOP Lab Project", this);
    QFont subtitleFont;
    subtitleFont.setPixelSize(13);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #7f8c8d;");
    mainLayout->addWidget(subtitleLabel);

    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    // --- Dashboard Overview subtitle ---
    QLabel* overviewLabel = new QLabel("Dashboard Overview", this);
    QFont overviewFont;
    overviewFont.setPixelSize(16);
    overviewFont.setBold(true);
    overviewLabel->setFont(overviewFont);
    mainLayout->addWidget(overviewLabel);

    // --- Stat cards row ---
    QHBoxLayout* cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(20);

    cardsLayout->addWidget(createStatCard("Total Hospitals",       "7", "#2e86de", hospitalsValueLabel));
    cardsLayout->addWidget(createStatCard("Total Doctors",         "21", "#27ae60", doctorsValueLabel));
    cardsLayout->addWidget(createStatCard("Active Patients",       "0", "#e67e22", activePatientsValueLabel));
    cardsLayout->addWidget(createStatCard("Discharged Patients",   "0", "#8e44ad", dischargedValueLabel));

    mainLayout->addLayout(cardsLayout);

    // --- Bottom stretch ---
    mainLayout->addStretch();

    refresh();
}

QWidget* DashboardPage::createStatCard(const QString& title, const QString& value,
                                        const QString& color, QLabel*& valueLabel)
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet(QString(
        "background-color: white;"
        "border-radius: 8px;"
        "border-left: 5px solid %1;"
    ).arg(color));
    card->setMinimumHeight(120);
    card->setMinimumWidth(200);

    QVBoxLayout* cardLayout = new QVBoxLayout(card);

    // Value label
    QLabel* valLabel = new QLabel(value, card);
    QFont valFont;
    valFont.setPixelSize(36);
    valFont.setBold(true);
    valLabel->setFont(valFont);
    valLabel->setStyleSheet(QString("color: %1;").arg(color));
    cardLayout->addWidget(valLabel);

    // Title label
    QLabel* titleLbl = new QLabel(title, card);
    QFont titleFont;
    titleFont.setPixelSize(13);
    titleLbl->setFont(titleFont);
    titleLbl->setStyleSheet("color: #7f8c8d;");
    cardLayout->addWidget(titleLbl);

    valueLabel = valLabel;
    return card;
}

void DashboardPage::refresh()
{
    hospitalsValueLabel->setText(QString::number(sysManager->getTotalHospitals()));
    doctorsValueLabel->setText(QString::number(sysManager->getTotalDoctors()));
    activePatientsValueLabel->setText(QString::number(sysManager->getActivePatientCount()));
    dischargedValueLabel->setText(QString::number(sysManager->getDischargedPatientCount()));
}
