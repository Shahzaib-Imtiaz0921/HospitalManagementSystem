#pragma once
#include "../domain/hospital.h"
#include "../domain/patient.h"
#include "../domain/consultationsession.h"
#include "../domain/proximityengine.h"
#include "../database/databasemanager.h"
#include <vector>
#include <QString>
#include <QVector>
#include <QPair>
#include <QList>
#include <QMap>

class SystemManager {
public:
    SystemManager();
    ~SystemManager() = default;

    // Rank hospitals by proximity score for a given sector
    // Returns sorted vector of (score, hospitalIndex) descending
    QVector<QPair<int,int>> rankHospitals(const QString& sector) const;

    // Register a new patient at the hospital at ranked index hospitalRankIdx
    // Returns the created Patient (with PID assigned and saved to DB)
    Patient registerPatient(const QString& name, int age,
                            const QString& loc, const QString& sym,
                            int hospitalIdx);

    // Assign an available doctor to the patient at the given hospital
    // Updates DB availability. Returns pointer to the doctor (nullptr if none available)
    Doctor* assignDoctor(Patient& patient, Hospital& hospital);

    // Conduct a consultation session between patient and doctor
    // Saves session to DB. Returns the session.
    ConsultationSession conductConsultation(Patient& patient, Doctor& doctor);

    // Discharge a patient: updates patient status and doctor availability in DB
    void dischargePatient(Patient& patient, Doctor& doctor);

    // Infer specialization from symptom text
    std::string inferSpec(const std::string& symptoms) const;

    // Data accessors for GUI
    std::vector<Hospital>& getHospitals();
    const std::vector<Hospital>& getHospitals() const;
    std::vector<Patient>& getPatients();
    const std::vector<Patient>& getPatients() const;
    DatabaseManager& getDB();

    // For GUI tables — returns Qt-friendly data
    QList<QMap<QString,QString>> getAllHospitalsData() const;
    QList<QMap<QString,QString>> getAllDoctorsData() const;
    QList<QMap<QString,QString>> getAllPatientsData() const;

    // Stats for dashboard
    int getTotalHospitals() const;
    int getTotalDoctors() const;
    int getActivePatientCount() const;
    int getDischargedPatientCount() const;

private:
    std::vector<Hospital>  hospitals;
    std::vector<Patient>   patients;
    ProximityEngine        pe;
    DatabaseManager        db;

    void loadFromDatabase();
};
