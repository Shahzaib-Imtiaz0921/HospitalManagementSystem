#include "consultationsession.h"
#include <iostream>

// Static counter starts at 5000 to match the original hospital_management.cpp
int ConsultationSession::s_counter = 5000;

// ── Constructors ──────────────────────────────────────────────────────────────

// Primary constructor: auto-generates SID
ConsultationSession::ConsultationSession(const std::string& pid,
                                         const std::string& did)
    : pid(pid), did(did), done(false)
{
    sid = "SES-" + std::to_string(++s_counter);
}

// DB-load constructor: all fields provided, no auto-generation
ConsultationSession::ConsultationSession(const std::string& sid,
                                         const std::string& pid,
                                         const std::string& did,
                                         const std::string& diag,
                                         const std::string& meds)
    : sid(sid), pid(pid), did(did), diag(diag), meds(meds), done(true)
{
}

// ── conduct ───────────────────────────────────────────────────────────────────
// Calls doc.diagnose(), stores the diagnosis and medicines, marks session done.

void ConsultationSession::conduct(const Doctor& doc, const std::string& symptoms) {
    auto result = doc.diagnose(symptoms);
    diag = result.first;
    meds = result.second;
    done = true;
}

// ── show ──────────────────────────────────────────────────────────────────────

void ConsultationSession::show() const {
    std::string bar(60, '=');
    std::string sep(60, '-');
    std::cout << bar << "\n"
              << "  SESSION ID  : " << sid  << "\n"
              << "  PATIENT ID  : " << pid  << "\n"
              << "  DOCTOR ID   : " << did  << "\n"
              << "  STATUS      : " << (done ? "Completed" : "Pending") << "\n"
              << sep << "\n"
              << "  DIAGNOSIS   : " << diag << "\n"
              << "  MEDICINES   : " << meds << "\n"
              << bar << "\n";
}

// ── Accessors ─────────────────────────────────────────────────────────────────

std::string ConsultationSession::getSID()  const { return sid; }
std::string ConsultationSession::getPID()  const { return pid; }
std::string ConsultationSession::getDID()  const { return did; }
std::string ConsultationSession::getDiag() const { return diag; }
std::string ConsultationSession::getMeds() const { return meds; }
bool        ConsultationSession::isDone()  const { return done; }
