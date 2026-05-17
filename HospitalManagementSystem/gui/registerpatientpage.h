#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "../domain/patient.h"
#include "../domain/doctor.h"

class SystemManager;

class RegisterPatientPage : public QWidget {
    Q_OBJECT
public:
    explicit RegisterPatientPage(SystemManager* sm, QWidget* parent = nullptr);

signals:
    void patientRegistered();

public slots:
    void refresh();

private slots:
    void onFindHospitals();
    void onRegisterAndProceed();

private:
    SystemManager*  sysManager;

    // Form fields
    QLineEdit*   nameEdit;
    QSpinBox*    ageSpin;
    QComboBox*   locationCombo;
    QTextEdit*   symptomsEdit;

    // Hospital ranking table
    QTableWidget* hospitalTable;
    QLabel*       recommendedLabel;

    // Buttons
    QPushButton* findBtn;
    QPushButton* registerBtn;

    // State
    int selectedHospitalIdx = -1;  // index into SystemManager::getHospitals()

    void setupForm(QWidget* formWidget);
    void setupHospitalTable(QWidget* tableWidget);
    void populateSectors();
};
