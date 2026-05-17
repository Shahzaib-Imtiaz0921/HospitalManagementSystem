#include "consultationresultdialog.h"
#include "../manager/systemmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QFont>

ConsultationResultDialog::ConsultationResultDialog(const ConsultationSession& session,
                                                   const Patient& patient,
                                                   const Doctor& doctor,
                                                   SystemManager* mgr,
                                                   QWidget* parent)
    : QDialog(parent)
    , m_session(session)
    , m_patient(patient)
    , m_doctor(doctor)
    , sysManager(mgr)
{
    setWindowTitle("Consultation Result");
    setMinimumWidth(520);
    setModal(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // --- Header ---
    QLabel* header = new QLabel("🩺 Consultation Summary", this);
    QFont headerFont;
    headerFont.setPixelSize(18);
    headerFont.setBold(true);
    header->setFont(headerFont);
    header->setStyleSheet("color: #1a2744;");
    mainLayout->addWidget(header);

    // --- Separator ---
    QFrame* sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep1);

    // --- Info grid ---
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(8);

    formLayout->addRow("Session ID:",    new QLabel(QString::fromStdString(session.getSID()), this));
    formLayout->addRow("Patient ID:",    new QLabel(QString::fromStdString(patient.getPID()), this));
    formLayout->addRow("Patient Name:",  new QLabel(QString::fromStdString(patient.getName()), this));
    formLayout->addRow("Doctor ID:",     new QLabel(QString::fromStdString(doctor.getID()), this));
    formLayout->addRow("Doctor Name:",   new QLabel(QString::fromStdString(doctor.getName()), this));
    formLayout->addRow("Specialization:", new QLabel(QString::fromStdString(doctor.getSpec()), this));

    mainLayout->addLayout(formLayout);

    // --- Separator ---
    QFrame* sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep2);

    // --- Diagnosis section ---
    QLabel* diagTitle = new QLabel("📋 Diagnosis", this);
    QFont boldFont;
    boldFont.setBold(true);
    diagTitle->setFont(boldFont);
    mainLayout->addWidget(diagTitle);

    QLabel* diagText = new QLabel(QString::fromStdString(session.getDiag()), this);
    diagText->setWordWrap(true);
    diagText->setStyleSheet("background-color: #e8f4fd; padding: 10px; border-radius: 4px; border-left: 4px solid #2e86de; color: #1a2744;");
    mainLayout->addWidget(diagText);

    // --- Medicines section ---
    QLabel* medsTitle = new QLabel("💊 Prescribed Medicines", this);
    medsTitle->setFont(boldFont);
    mainLayout->addWidget(medsTitle);

    QLabel* medsText = new QLabel(QString::fromStdString(session.getMeds()), this);
    medsText->setWordWrap(true);
    medsText->setStyleSheet("background-color: #e8f8f0; padding: 10px; border-radius: 4px; border-left: 4px solid #27ae60; color: #1a2744;");
    mainLayout->addWidget(medsText);

    // --- Button row ---
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* dischargeBtn = new QPushButton("🏠 Discharge Patient", this);
    dischargeBtn->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 8px 20px; border-radius: 4px;");
    connect(dischargeBtn, &QPushButton::clicked, this, &ConsultationResultDialog::onDischarge);
    btnLayout->addWidget(dischargeBtn);

    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(closeBtn);

    mainLayout->addLayout(btnLayout);
}

void ConsultationResultDialog::onDischarge()
{
    sysManager->dischargePatient(m_patient, m_doctor);
    QMessageBox::information(this, "Discharged",
        QString("Patient %1 has been successfully discharged.")
            .arg(QString::fromStdString(m_patient.getName())));
    accept();
}
