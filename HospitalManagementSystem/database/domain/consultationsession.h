#pragma once
#include "doctor.h"
#include <string>

// ConsultationSession records a single patient–doctor interaction.
// Demonstrates: Encapsulation, static counter for auto-ID generation
class ConsultationSession {
public:
    // Default constructor (needed for container storage)
    ConsultationSession() = default;

    // Primary constructor — auto-generates SID as "SES-XXXX" (counter starts at 5000)
    ConsultationSession(const std::string& pid, const std::string& did);

    // DB-load constructor — all fields provided (no auto-generation)
    ConsultationSession(const std::string& sid, const std::string& pid,
                        const std::string& did, const std::string& diag,
                        const std::string& meds);

    // Conduct the session: calls doc.diagnose(symptoms), stores result, marks done
    void conduct(const Doctor& doc, const std::string& symptoms);

    // Print session summary to stdout
    void show() const;

    // Accessors
    std::string getSID()  const;
    std::string getPID()  const;
    std::string getDID()  const;
    std::string getDiag() const;
    std::string getMeds() const;
    bool        isDone()  const;

private:
    std::string sid;
    std::string pid;
    std::string did;
    std::string diag;
    std::string meds;
    bool        done = false;

    // Auto-ID counter shared across all ConsultationSession instances
    static int s_counter;
};
