#pragma once
#include <QString>
#include <QList>
#include <QMap>
#include <QSqlDatabase>
#include <vector>
#include "../domain/doctor.h"
#include "../domain/patient.h"
#include "../domain/hospital.h"
#include "../domain/consultationsession.h"
#include "../domain/proximityengine.h"

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // Opens hms_data.db, creates tables, seeds data if needed
    bool initialize();

    // Check if hospitals table already has rows
    bool isSeeded() const;

    // Seed the 7 hospitals and 21 doctors
    void seedData();

    // Patient CRUD — domain-object overload (used by SystemManager)
    bool savePatient(Patient& p);
    bool savePatient(const QString& pid, const QString& name, int age,
                     const QString& loc, const QString& sym,
                     const QString& hospitalId, const QString& doctorId,
                     const QString& status = "Active");
    bool updatePatientStatus(const std::string& pid, const std::string& status);
    bool updatePatientStatus(const QString& pid, const QString& status);

    // Session CRUD — domain-object overload (used by SystemManager)
    bool saveSession(ConsultationSession& s);
    bool saveSession(const QString& sid, const QString& pid, const QString& did,
                     const QString& diag, const QString& meds,
                     const QString& status = "Completed");

    // Doctor availability
    bool updateDoctorAvailability(const std::string& did, bool available);
    bool updateDoctorAvailability(const QString& did, bool available);

    // Load methods — return Qt map lists (used by GUI pages)
    QList<QMap<QString,QString>> loadPatients() const;
    QList<QMap<QString,QString>> loadSessions() const;
    QList<QMap<QString,QString>> loadDoctors() const;
    QList<QMap<QString,QString>> loadHospitals() const;

    // Load methods — return domain objects (used by SystemManager)
    std::vector<Patient> loadPatientsAsObjects() const;
    std::vector<Doctor>  loadDoctorsAsObjects() const;

    // CSV export
    bool exportPatientsCSV(const QString& filePath) const;

private:
    QSqlDatabase db;
    bool createTables();
};
