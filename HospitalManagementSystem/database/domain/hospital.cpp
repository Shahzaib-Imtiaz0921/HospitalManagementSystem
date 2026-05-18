#include "hospital.h"
#include <iostream>
#include <iomanip>

// ── Constructor ───────────────────────────────────────────────────────────────

Hospital::Hospital(const std::string& hid, const std::string& name,
                   const std::string& loc, int cap, const std::string& spec)
    : hid(hid), name(name), loc(loc), spec(spec), cap(cap)
{
}

// ── Mutators ──────────────────────────────────────────────────────────────────

void Hospital::addDoc(const Doctor& d) {
    docs.push_back(d);
}

void Hospital::regPat(const std::string& pid) {
    pids.push_back(pid);
}

// ── findDoc ───────────────────────────────────────────────────────────────────
// Prefers a doctor whose specialization matches pref; falls back to any
// available doctor. Returns nullptr if no doctor is available.

Doctor* Hospital::findDoc(const std::string& pref) {
    // First pass: prefer matching specialization
    if (!pref.empty()) {
        for (auto& d : docs) {
            if (d.isFree() && d.getSpec() == pref) {
                return &d;
            }
        }
    }
    // Second pass: any available doctor
    for (auto& d : docs) {
        if (d.isFree()) {
            return &d;
        }
    }
    return nullptr;
}

// ── displayInfo ───────────────────────────────────────────────────────────────

void Hospital::displayInfo() const {
    std::cout << "  Hospital ID : " << hid          << "\n"
              << "  Name        : " << name         << "\n"
              << "  Location    : " << loc          << "\n"
              << "  Capacity    : " << cap          << " beds\n"
              << "  Specialty   : " << spec         << "\n"
              << "  Doctors     : " << docs.size()  << "\n";
}

// ── listDocs ──────────────────────────────────────────────────────────────────

void Hospital::listDocs() const {
    std::string bar(60, '-');
    std::cout << bar << "\n";
    std::cout << "  Doctors at " << name << ":\n";
    std::cout << "  | " << std::left << std::setw(10) << "ID"
              << " | " << std::setw(26) << "Name"
              << " | " << std::setw(18) << "Specialization"
              << " | Status\n";
    std::cout << bar << "\n";
    for (const auto& d : docs) {
        d.displayInfo();
    }
    std::cout << bar << "\n";
}

// ── Accessors ─────────────────────────────────────────────────────────────────

std::string Hospital::getHID()      const { return hid; }
std::string Hospital::getName()     const { return name; }
std::string Hospital::getLoc()      const { return loc; }
std::string Hospital::getSpec()     const { return spec; }
int         Hospital::getCap()      const { return cap; }
int         Hospital::getDocCount() const { return static_cast<int>(docs.size()); }

const std::vector<Doctor>& Hospital::getDocs() const { return docs; }
std::vector<Doctor>&       Hospital::getDocs()       { return docs; }
