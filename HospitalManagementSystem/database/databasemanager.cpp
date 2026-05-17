#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

DatabaseManager::DatabaseManager()
{
    // Use a named connection so multiple instances don't clash
    db = QSqlDatabase::addDatabase("QSQLITE", "hms_connection");
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
        db.close();
    // Remove the named connection to avoid Qt warnings on re-open
    QSqlDatabase::removeDatabase("hms_connection");
}

// ---------------------------------------------------------------------------
// initialize()
// ---------------------------------------------------------------------------

bool DatabaseManager::initialize()
{
    db.setDatabaseName("hms_data.db");

    if (!db.open()) {
        qDebug() << "DB Error: could not open hms_data.db:" << db.lastError().text();
        return false;
    }

    if (!createTables()) {
        qDebug() << "DB Error: failed to create tables";
        return false;
    }

    if (!isSeeded()) {
        seedData();
    }

    return true;
}

// ---------------------------------------------------------------------------
// createTables()
// ---------------------------------------------------------------------------

bool DatabaseManager::createTables()
{
    QSqlQuery query(db);

    const QString createHospitals = R"(
        CREATE TABLE IF NOT EXISTS hospitals (
            hid  TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            loc  TEXT NOT NULL,
            cap  INTEGER NOT NULL,
            spec TEXT NOT NULL
        )
    )";

    const QString createDoctors = R"(
        CREATE TABLE IF NOT EXISTS doctors (
            did         TEXT PRIMARY KEY,
            name        TEXT NOT NULL,
            age         INTEGER NOT NULL,
            spec        TEXT NOT NULL,
            hospital_id TEXT NOT NULL,
            available   INTEGER NOT NULL DEFAULT 1
        )
    )";

    const QString createPatients = R"(
        CREATE TABLE IF NOT EXISTS patients (
            pid         TEXT PRIMARY KEY,
            name        TEXT NOT NULL,
            age         INTEGER NOT NULL,
            location    TEXT NOT NULL,
            symptoms    TEXT NOT NULL,
            hospital_id TEXT,
            doctor_id   TEXT,
            status      TEXT NOT NULL DEFAULT 'Active'
        )
    )";

    const QString createSessions = R"(
        CREATE TABLE IF NOT EXISTS sessions (
            sid        TEXT PRIMARY KEY,
            patient_id TEXT NOT NULL,
            doctor_id  TEXT NOT NULL,
            diagnosis  TEXT NOT NULL,
            medicines  TEXT NOT NULL,
            status     TEXT NOT NULL DEFAULT 'Completed'
        )
    )";

    if (!query.exec(createHospitals)) {
        qDebug() << "DB Error (create hospitals):" << query.lastError().text();
        return false;
    }
    if (!query.exec(createDoctors)) {
        qDebug() << "DB Error (create doctors):" << query.lastError().text();
        return false;
    }
    if (!query.exec(createPatients)) {
        qDebug() << "DB Error (create patients):" << query.lastError().text();
        return false;
    }
    if (!query.exec(createSessions)) {
        qDebug() << "DB Error (create sessions):" << query.lastError().text();
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// isSeeded()
// ---------------------------------------------------------------------------

bool DatabaseManager::isSeeded() const
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM hospitals");
    if (!query.exec()) {
        qDebug() << "DB Error (isSeeded):" << query.lastError().text();
        return false;
    }
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

// ---------------------------------------------------------------------------
// seedData()  — 7 hospitals + 21 doctors
// ---------------------------------------------------------------------------

void DatabaseManager::seedData()
{
    QSqlQuery query(db);

    // ---- Hospitals ----
    struct HospitalRow { QString hid, name, loc, spec; int cap; };
    const QList<HospitalRow> hospitals = {
        { "H001", "PIMS",                        "G-8/Margalla Rd",       "General Medicine", 800 },
        { "H002", "Polyclinic Hospital",          "G-6/Civic Centre",      "General Medicine", 600 },
        { "H003", "Shifa International Hospital", "H-8/Pitras Bukhari Rd", "Cardiology",       500 },
        { "H004", "Maroof Hospital",              "F-10/Markaz",           "Orthopedics",      350 },
        { "H005", "Ali Medical Centre",           "F-8/Markaz",            "General Medicine", 250 },
        { "H006", "Kulsum International Hospital","F-8/Blue Area",         "Neurology",        300 },
        { "H007", "NESCOM Hospital",              "H-11/Islamabad",        "General Medicine", 200 },
    };

    query.prepare("INSERT OR IGNORE INTO hospitals (hid, name, loc, cap, spec) "
                  "VALUES (:hid, :name, :loc, :cap, :spec)");
    for (const auto& h : hospitals) {
        query.bindValue(":hid",  h.hid);
        query.bindValue(":name", h.name);
        query.bindValue(":loc",  h.loc);
        query.bindValue(":cap",  h.cap);
        query.bindValue(":spec", h.spec);
        if (!query.exec())
            qDebug() << "DB Error (seed hospital" << h.hid << "):" << query.lastError().text();
    }

    // ---- Doctors ----
    struct DoctorRow { QString did, name, hid, spec; int age; };
    const QList<DoctorRow> doctors = {
        // H001 — PIMS
        { "DOC-201", "Dr. Ayesha Malik",   "H001", "General Medicine", 45 },
        { "DOC-202", "Dr. Tariq Hussain",  "H001", "Cardiology",       50 },
        { "DOC-203", "Dr. Sana Baig",      "H001", "Neurology",        38 },
        // H002 — Polyclinic
        { "DOC-204", "Dr. Imran Shah",     "H002", "General Medicine", 42 },
        { "DOC-205", "Dr. Nadia Qureshi",  "H002", "Pediatrics",       36 },
        { "DOC-206", "Dr. Faisal Raza",    "H002", "ENT",              48 },
        // H003 — Shifa
        { "DOC-207", "Dr. Kamran Akhtar",  "H003", "Cardiology",       55 },
        { "DOC-208", "Dr. Zara Noor",      "H003", "Neurology",        40 },
        { "DOC-209", "Dr. Bilal Ahmed",    "H003", "Orthopedics",      44 },
        // H004 — Maroof
        { "DOC-210", "Dr. Hina Javed",     "H004", "Orthopedics",      37 },
        { "DOC-211", "Dr. Usman Farooq",   "H004", "General Medicine", 46 },
        { "DOC-212", "Dr. Rabia Saleem",   "H004", "Dermatology",      33 },
        // H005 — Ali Medical
        { "DOC-213", "Dr. Ali Hassan",     "H005", "General Medicine", 39 },
        { "DOC-214", "Dr. Mehwish Iqbal",  "H005", "Pediatrics",       34 },
        { "DOC-215", "Dr. Shahid Mehmood", "H005", "ENT",              51 },
        // H006 — Kulsum
        { "DOC-216", "Dr. Saima Riaz",     "H006", "Neurology",        43 },
        { "DOC-217", "Dr. Junaid Mirza",   "H006", "Cardiology",       47 },
        { "DOC-218", "Dr. Amna Khalid",    "H006", "General Medicine", 35 },
        // H007 — NESCOM
        { "DOC-219", "Dr. Waqar Zafar",    "H007", "General Medicine", 41 },
        { "DOC-220", "Dr. Lubna Arif",     "H007", "Pediatrics",       38 },
        { "DOC-221", "Dr. Asad Mehmood",   "H007", "Orthopedics",      52 },
    };

    query.prepare("INSERT OR IGNORE INTO doctors (did, name, age, spec, hospital_id, available) "
                  "VALUES (:did, :name, :age, :spec, :hid, 1)");
    for (const auto& d : doctors) {
        query.bindValue(":did",  d.did);
        query.bindValue(":name", d.name);
        query.bindValue(":age",  d.age);
        query.bindValue(":spec", d.spec);
        query.bindValue(":hid",  d.hid);
        if (!query.exec())
            qDebug() << "DB Error (seed doctor" << d.did << "):" << query.lastError().text();
    }
}

// ---------------------------------------------------------------------------
// Patient CRUD
// ---------------------------------------------------------------------------

bool DatabaseManager::savePatient(Patient& p)
{
    return savePatient(
        QString::fromStdString(p.getPID()),
        QString::fromStdString(p.getName()),
        p.getAge(),
        QString::fromStdString(p.getLoc()),
        QString::fromStdString(p.getSym()),
        QString::fromStdString(p.getHID()),
        QString::fromStdString(p.getDID()),
        p.isDischarged() ? "Discharged" : "Active"
    );
}

bool DatabaseManager::savePatient(const QString& pid, const QString& name, int age,
                                   const QString& loc, const QString& sym,
                                   const QString& hospitalId, const QString& doctorId,
                                   const QString& status)
{
    QSqlQuery query(db);
    query.prepare(
        "INSERT OR REPLACE INTO patients "
        "(pid, name, age, location, symptoms, hospital_id, doctor_id, status) "
        "VALUES (:pid, :name, :age, :loc, :sym, :hid, :did, :status)"
    );
    query.bindValue(":pid",    pid);
    query.bindValue(":name",   name);
    query.bindValue(":age",    age);
    query.bindValue(":loc",    loc);
    query.bindValue(":sym",    sym);
    query.bindValue(":hid",    hospitalId);
    query.bindValue(":did",    doctorId);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "DB Error (savePatient):" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updatePatientStatus(const std::string& pid, const std::string& status)
{
    return updatePatientStatus(
        QString::fromStdString(pid),
        QString::fromStdString(status)
    );
}

bool DatabaseManager::updatePatientStatus(const QString& pid, const QString& status)
{
    QSqlQuery query(db);
    query.prepare("UPDATE patients SET status = :status WHERE pid = :pid");
    query.bindValue(":status", status);
    query.bindValue(":pid",    pid);

    if (!query.exec()) {
        qDebug() << "DB Error (updatePatientStatus):" << query.lastError().text();
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Session CRUD
// ---------------------------------------------------------------------------

bool DatabaseManager::saveSession(ConsultationSession& s)
{
    return saveSession(
        QString::fromStdString(s.getSID()),
        QString::fromStdString(s.getPID()),
        QString::fromStdString(s.getDID()),
        QString::fromStdString(s.getDiag()),
        QString::fromStdString(s.getMeds()),
        s.isDone() ? "Completed" : "Pending"
    );
}

bool DatabaseManager::saveSession(const QString& sid, const QString& pid, const QString& did,
                                   const QString& diag, const QString& meds,
                                   const QString& status)
{
    QSqlQuery query(db);
    query.prepare(
        "INSERT OR REPLACE INTO sessions "
        "(sid, patient_id, doctor_id, diagnosis, medicines, status) "
        "VALUES (:sid, :pid, :did, :diag, :meds, :status)"
    );
    query.bindValue(":sid",    sid);
    query.bindValue(":pid",    pid);
    query.bindValue(":did",    did);
    query.bindValue(":diag",   diag);
    query.bindValue(":meds",   meds);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qDebug() << "DB Error (saveSession):" << query.lastError().text();
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Doctor availability
// ---------------------------------------------------------------------------

bool DatabaseManager::updateDoctorAvailability(const std::string& did, bool available)
{
    return updateDoctorAvailability(QString::fromStdString(did), available);
}

bool DatabaseManager::updateDoctorAvailability(const QString& did, bool available)
{
    QSqlQuery query(db);
    query.prepare("UPDATE doctors SET available = :avail WHERE did = :did");
    query.bindValue(":avail", available ? 1 : 0);
    query.bindValue(":did",   did);

    if (!query.exec()) {
        qDebug() << "DB Error (updateDoctorAvailability):" << query.lastError().text();
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Load methods — Qt map lists (used by GUI)
// ---------------------------------------------------------------------------

QList<QMap<QString,QString>> DatabaseManager::loadPatients() const
{
    QList<QMap<QString,QString>> result;
    QSqlQuery query(db);
    query.prepare("SELECT pid, name, age, location, symptoms, hospital_id, doctor_id, status "
                  "FROM patients");
    if (!query.exec()) {
        qDebug() << "DB Error (loadPatients):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        QMap<QString,QString> row;
        row["pid"]         = query.value("pid").toString();
        row["name"]        = query.value("name").toString();
        row["age"]         = query.value("age").toString();
        row["location"]    = query.value("location").toString();
        row["symptoms"]    = query.value("symptoms").toString();
        row["hospital_id"] = query.value("hospital_id").toString();
        row["doctor_id"]   = query.value("doctor_id").toString();
        row["status"]      = query.value("status").toString();
        result.append(row);
    }
    return result;
}

QList<QMap<QString,QString>> DatabaseManager::loadSessions() const
{
    QList<QMap<QString,QString>> result;
    QSqlQuery query(db);
    query.prepare("SELECT sid, patient_id, doctor_id, diagnosis, medicines, status "
                  "FROM sessions");
    if (!query.exec()) {
        qDebug() << "DB Error (loadSessions):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        QMap<QString,QString> row;
        row["sid"]        = query.value("sid").toString();
        row["patient_id"] = query.value("patient_id").toString();
        row["doctor_id"]  = query.value("doctor_id").toString();
        row["diagnosis"]  = query.value("diagnosis").toString();
        row["medicines"]  = query.value("medicines").toString();
        row["status"]     = query.value("status").toString();
        result.append(row);
    }
    return result;
}

QList<QMap<QString,QString>> DatabaseManager::loadDoctors() const
{
    QList<QMap<QString,QString>> result;
    QSqlQuery query(db);
    query.prepare("SELECT did, name, age, spec, hospital_id, available FROM doctors");
    if (!query.exec()) {
        qDebug() << "DB Error (loadDoctors):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        QMap<QString,QString> row;
        row["did"]         = query.value("did").toString();
        row["name"]        = query.value("name").toString();
        row["age"]         = query.value("age").toString();
        row["spec"]        = query.value("spec").toString();
        row["hospital_id"] = query.value("hospital_id").toString();
        row["available"]   = query.value("available").toString();
        result.append(row);
    }
    return result;
}

QList<QMap<QString,QString>> DatabaseManager::loadHospitals() const
{
    QList<QMap<QString,QString>> result;
    QSqlQuery query(db);
    query.prepare("SELECT hid, name, loc, cap, spec FROM hospitals");
    if (!query.exec()) {
        qDebug() << "DB Error (loadHospitals):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        QMap<QString,QString> row;
        row["hid"]  = query.value("hid").toString();
        row["name"] = query.value("name").toString();
        row["loc"]  = query.value("loc").toString();
        row["cap"]  = query.value("cap").toString();
        row["spec"] = query.value("spec").toString();
        result.append(row);
    }
    return result;
}

// ---------------------------------------------------------------------------
// Load methods — domain objects (used by SystemManager)
// ---------------------------------------------------------------------------

std::vector<Patient> DatabaseManager::loadPatientsAsObjects() const
{
    std::vector<Patient> result;
    QSqlQuery query(db);
    query.prepare("SELECT pid, name, age, location, symptoms FROM patients");
    if (!query.exec()) {
        qDebug() << "DB Error (loadPatientsAsObjects):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        Patient p(
            query.value("pid").toString().toStdString(),
            query.value("name").toString().toStdString(),
            query.value("age").toInt(),
            query.value("location").toString().toStdString(),
            query.value("symptoms").toString().toStdString()
        );
        // Restore hospital/doctor assignment and discharge status via separate query
        QSqlQuery sub(db);
        sub.prepare("SELECT hospital_id, doctor_id, status FROM patients WHERE pid = :pid");
        sub.bindValue(":pid", query.value("pid").toString());
        if (sub.exec() && sub.next()) {
            p.setHID(sub.value("hospital_id").toString().toStdString());
            p.setDID(sub.value("doctor_id").toString().toStdString());
            if (sub.value("status").toString() == "Discharged")
                p.discharge();
        }
        result.push_back(p);
    }
    return result;
}

std::vector<Doctor> DatabaseManager::loadDoctorsAsObjects() const
{
    std::vector<Doctor> result;
    QSqlQuery query(db);
    query.prepare("SELECT did, name, age, spec, hospital_id, available FROM doctors");
    if (!query.exec()) {
        qDebug() << "DB Error (loadDoctorsAsObjects):" << query.lastError().text();
        return result;
    }
    while (query.next()) {
        Doctor d(
            query.value("did").toString().toStdString(),
            query.value("name").toString().toStdString(),
            query.value("age").toInt(),
            query.value("spec").toString().toStdString(),
            query.value("hospital_id").toString().toStdString()
        );
        d.setFree(query.value("available").toInt() == 1);
        result.push_back(d);
    }
    return result;
}

// ---------------------------------------------------------------------------
// CSV export
// ---------------------------------------------------------------------------

bool DatabaseManager::exportPatientsCSV(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "DB Error (exportPatientsCSV): cannot open file" << filePath;
        return false;
    }

    QTextStream out(&file);

    // Header row
    out << "Patient ID,Name,Age,Location,Symptoms,Hospital,Doctor,Status\n";

    QSqlQuery query(db);
    query.prepare(
        "SELECT p.pid, p.name, p.age, p.location, p.symptoms, "
        "       COALESCE(h.name, p.hospital_id) AS hospital, "
        "       COALESCE(d.name, p.doctor_id)   AS doctor, "
        "       p.status "
        "FROM patients p "
        "LEFT JOIN hospitals h ON p.hospital_id = h.hid "
        "LEFT JOIN doctors   d ON p.doctor_id   = d.did"
    );

    if (!query.exec()) {
        qDebug() << "DB Error (exportPatientsCSV query):" << query.lastError().text();
        file.close();
        return false;
    }

    while (query.next()) {
        // Wrap fields that may contain commas in double-quotes
        auto escape = [](const QString& s) -> QString {
            if (s.contains(',') || s.contains('"') || s.contains('\n')) {
                return "\"" + QString(s).replace("\"", "\"\"") + "\"";
            }
            return s;
        };

        out << escape(query.value("pid").toString())      << ","
            << escape(query.value("name").toString())     << ","
            << query.value("age").toString()              << ","
            << escape(query.value("location").toString()) << ","
            << escape(query.value("symptoms").toString()) << ","
            << escape(query.value("hospital").toString()) << ","
            << escape(query.value("doctor").toString())   << ","
            << escape(query.value("status").toString())   << "\n";
    }

    file.close();
    return true;
}
