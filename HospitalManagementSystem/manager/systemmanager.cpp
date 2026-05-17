#include "systemmanager.h"
#include <algorithm>
#include <cctype>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

SystemManager::SystemManager()
{
    db.initialize();
    loadFromDatabase();
}

// ---------------------------------------------------------------------------
// loadFromDatabase()
// ---------------------------------------------------------------------------

void SystemManager::loadFromDatabase()
{
    // 1. Load hospitals from DB
    QList<QMap<QString,QString>> hospitalRows = db.loadHospitals();
    hospitals.clear();
    for (const auto& row : hospitalRows) {
        Hospital h(
            row["hid"].toStdString(),
            row["name"].toStdString(),
            row["loc"].toStdString(),
            row["cap"].toInt(),
            row["spec"].toStdString()
        );
        hospitals.push_back(h);
    }

    // 2. Load doctors and add each to the correct hospital
    std::vector<Doctor> allDoctors = db.loadDoctorsAsObjects();
    for (const Doctor& doc : allDoctors) {
        for (Hospital& hospital : hospitals) {
            if (hospital.getHID() == doc.getHospitalId()) {
                hospital.addDoc(doc);
                break;
            }
        }
    }

    // 3. Load patients
    patients = db.loadPatientsAsObjects();
}

// ---------------------------------------------------------------------------
// rankHospitals()
// ---------------------------------------------------------------------------

QVector<QPair<int,int>> SystemManager::rankHospitals(const QString& sector) const
{
    QVector<QPair<int,int>> ranked;
    for (int i = 0; i < static_cast<int>(hospitals.size()); ++i) {
        int s = pe.score(hospitals[i].getName(), sector.toStdString());
        ranked.append(qMakePair(s, i));
    }
    // Sort descending by score
    std::sort(ranked.begin(), ranked.end(),
              [](const QPair<int,int>& a, const QPair<int,int>& b) {
                  return a.first > b.first;
              });
    return ranked;
}

// ---------------------------------------------------------------------------
// registerPatient()
// ---------------------------------------------------------------------------

Patient SystemManager::registerPatient(const QString& name, int age,
                                        const QString& loc, const QString& sym,
                                        int hospitalIdx)
{
    Patient patient(name.toStdString(), age,
                    loc.toStdString(), sym.toStdString());

    patient.setHID(hospitals[hospitalIdx].getHID());
    hospitals[hospitalIdx].regPat(patient.getPID());

    db.savePatient(patient);
    patients.push_back(patient);

    return patient;
}

// ---------------------------------------------------------------------------
// assignDoctor()
// ---------------------------------------------------------------------------

Doctor* SystemManager::assignDoctor(Patient& patient, Hospital& hospital)
{
    std::string preferredSpec = inferSpec(patient.getSym());
    Doctor* doctor = hospital.findDoc(preferredSpec);

    if (!doctor) {
        return nullptr;
    }

    doctor->setFree(false);
    patient.setDID(doctor->getID());

    db.updateDoctorAvailability(doctor->getID(), false);
    db.savePatient(patient);

    return doctor;
}

// ---------------------------------------------------------------------------
// conductConsultation()
// ---------------------------------------------------------------------------

ConsultationSession SystemManager::conductConsultation(Patient& patient, Doctor& doctor)
{
    ConsultationSession session(patient.getPID(), doctor.getID());
    session.conduct(doctor, patient.getSym());
    db.saveSession(session);
    return session;
}

// ---------------------------------------------------------------------------
// dischargePatient()
// ---------------------------------------------------------------------------

void SystemManager::dischargePatient(Patient& patient, Doctor& doctor)
{
    patient.discharge();
    doctor.setFree(true);

    db.updatePatientStatus(patient.getPID(), "Discharged");
    db.updateDoctorAvailability(doctor.getID(), true);

    // Update the patient in the patients vector (find by PID and update status)
    for (Patient& p : patients) {
        if (p.getPID() == patient.getPID()) {
            p.discharge();
            break;
        }
    }
}

// ---------------------------------------------------------------------------
// inferSpec()
// ---------------------------------------------------------------------------

std::string SystemManager::inferSpec(const std::string& symptoms) const
{
    // Convert to lowercase for case-insensitive matching
    std::string s = symptoms;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s.find("heart")   != std::string::npos ||
        s.find("chest")   != std::string::npos ||
        s.find("cardiac") != std::string::npos)
        return "Cardiology";

    if (s.find("head")    != std::string::npos ||
        s.find("brain")   != std::string::npos ||
        s.find("migraine")!= std::string::npos)
        return "Neurology";

    if (s.find("bone")    != std::string::npos ||
        s.find("joint")   != std::string::npos ||
        s.find("fracture")!= std::string::npos)
        return "Orthopedics";

    if (s.find("child")   != std::string::npos ||
        s.find("fever")   != std::string::npos ||
        s.find("kid")     != std::string::npos)
        return "Pediatrics";

    if (s.find("ear")     != std::string::npos ||
        s.find("nose")    != std::string::npos ||
        s.find("throat")  != std::string::npos)
        return "ENT";

    if (s.find("skin")    != std::string::npos ||
        s.find("rash")    != std::string::npos ||
        s.find("itch")    != std::string::npos)
        return "Dermatology";

    return "General Medicine";
}

// ---------------------------------------------------------------------------
// Data accessors
// ---------------------------------------------------------------------------

std::vector<Hospital>& SystemManager::getHospitals()
{
    return hospitals;
}

const std::vector<Hospital>& SystemManager::getHospitals() const
{
    return hospitals;
}

std::vector<Patient>& SystemManager::getPatients()
{
    return patients;
}

const std::vector<Patient>& SystemManager::getPatients() const
{
    return patients;
}

DatabaseManager& SystemManager::getDB()
{
    return db;
}

// ---------------------------------------------------------------------------
// getAllHospitalsData()
// ---------------------------------------------------------------------------

QList<QMap<QString,QString>> SystemManager::getAllHospitalsData() const
{
    QList<QMap<QString,QString>> result = db.loadHospitals();

    // Count doctors per hospital from DB
    QList<QMap<QString,QString>> allDoctors = db.loadDoctors();

    for (auto& row : result) {
        QString hid = row["hid"];
        int count = 0;
        for (const auto& doc : allDoctors) {
            if (doc["hospital_id"] == hid)
                ++count;
        }
        row["doc_count"] = QString::number(count);
    }

    return result;
}

// ---------------------------------------------------------------------------
// getAllDoctorsData()
// ---------------------------------------------------------------------------

QList<QMap<QString,QString>> SystemManager::getAllDoctorsData() const
{
    QList<QMap<QString,QString>> result = db.loadDoctors();

    // Build a hospital id → name lookup map
    QList<QMap<QString,QString>> allHospitals = db.loadHospitals();
    QMap<QString,QString> hospitalNameMap;
    for (const auto& h : allHospitals) {
        hospitalNameMap[h["hid"]] = h["name"];
    }

    for (auto& row : result) {
        QString hid = row["hospital_id"];
        row["hospital_name"] = hospitalNameMap.value(hid, hid);
    }

    return result;
}

// ---------------------------------------------------------------------------
// getAllPatientsData()
// ---------------------------------------------------------------------------

QList<QMap<QString,QString>> SystemManager::getAllPatientsData() const
{
    QList<QMap<QString,QString>> result = db.loadPatients();

    // Build lookup maps
    QList<QMap<QString,QString>> allHospitals = db.loadHospitals();
    QList<QMap<QString,QString>> allDoctors   = db.loadDoctors();

    QMap<QString,QString> hospitalNameMap;
    for (const auto& h : allHospitals) {
        hospitalNameMap[h["hid"]] = h["name"];
    }

    QMap<QString,QString> doctorNameMap;
    for (const auto& d : allDoctors) {
        doctorNameMap[d["did"]] = d["name"];
    }

    for (auto& row : result) {
        row["hospital_name"] = hospitalNameMap.value(row["hospital_id"], row["hospital_id"]);
        row["doctor_name"]   = doctorNameMap.value(row["doctor_id"],   row["doctor_id"]);
    }

    return result;
}

// ---------------------------------------------------------------------------
// Stats
// ---------------------------------------------------------------------------

int SystemManager::getTotalHospitals() const
{
    return static_cast<int>(hospitals.size());
}

int SystemManager::getTotalDoctors() const
{
    return db.loadDoctors().size();
}

int SystemManager::getActivePatientCount() const
{
    QList<QMap<QString,QString>> allPatients = db.loadPatients();
    int count = 0;
    for (const auto& p : allPatients) {
        if (p["status"] == "Active")
            ++count;
    }
    return count;
}

int SystemManager::getDischargedPatientCount() const
{
    QList<QMap<QString,QString>> allPatients = db.loadPatients();
    int count = 0;
    for (const auto& p : allPatients) {
        if (p["status"] == "Discharged")
            ++count;
    }
    return count;
}
