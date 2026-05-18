#include "patient.h"
#include <iostream>

// Static counter starts at 1000 to match the original hospital_management.cpp
int Patient::s_counter = 1000;

// ── Constructors ──────────────────────────────────────────────────────────────

Patient::Patient(const std::string& name, int age,
                 const std::string& loc, const std::string& sym,
                 const std::string& pid)
    : Person(name, age), loc(loc), sym(sym), disc(false)
{
    if (pid.empty()) {
        this->pid = "PAT-" + std::to_string(++s_counter);
    } else {
        this->pid = pid;
    }
}

Patient::Patient(const std::string& pid, const std::string& name, int age,
                 const std::string& loc, const std::string& sym)
    : Person(name, age), pid(pid), loc(loc), sym(sym), disc(false)
{
    // ID is provided (e.g. loaded from database) — no auto-generation needed
}

// ── Accessors ─────────────────────────────────────────────────────────────────

std::string Patient::getPID()  const { return pid; }
std::string Patient::getLoc()  const { return loc; }
std::string Patient::getSym()  const { return sym; }
bool        Patient::isDischarged() const { return disc; }
bool        Patient::isDone()  const { return disc; }
std::string Patient::getHID()  const { return hid; }
std::string Patient::getDID()  const { return did; }

void Patient::setHID(const std::string& h) { hid = h; }
void Patient::setDID(const std::string& d) { did = d; }
void Patient::discharge()                  { disc = true; }

// ── displayInfo ───────────────────────────────────────────────────────────────

void Patient::displayInfo() const {
    std::cout << "  Patient ID  : " << pid                              << "\n"
              << "  Name        : " << name                             << "\n"
              << "  Age         : " << age                              << "\n"
              << "  Location    : " << loc                              << "\n"
              << "  Symptoms    : " << sym                              << "\n"
              << "  Hospital    : " << (hid.empty() ? "N/A" : hid)     << "\n"
              << "  Doctor      : " << (did.empty() ? "N/A" : did)     << "\n"
              << "  Status      : " << (disc ? "Discharged" : "Active") << "\n";
}
