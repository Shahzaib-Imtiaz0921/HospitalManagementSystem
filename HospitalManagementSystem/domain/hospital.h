#pragma once
#include "doctor.h"
#include <string>
#include <vector>

// Hospital class — NOT inheriting from Person.
// Demonstrates: Encapsulation (private fields, public accessors)
class Hospital {
public:
    // Default constructor (needed for container storage)
    Hospital() = default;

    // Primary constructor
    Hospital(const std::string& hid, const std::string& name,
             const std::string& loc, int cap, const std::string& spec);

    // Add a doctor to this hospital
    void addDoc(const Doctor& d);

    // Register a patient PID at this hospital
    void regPat(const std::string& pid);

    // Find an available doctor; prefers one matching pref specialization,
    // falls back to any available doctor. Returns nullptr if none available.
    Doctor* findDoc(const std::string& pref = "");

    // Display hospital summary to stdout
    void displayInfo() const;

    // List all doctors at this hospital to stdout
    void listDocs() const;

    // Accessors
    std::string getHID()      const;
    std::string getName()     const;
    std::string getLoc()      const;
    std::string getSpec()     const;
    int         getCap()      const;
    int         getDocCount() const;

    // Doctor collection accessors
    const std::vector<Doctor>& getDocs() const;
    std::vector<Doctor>&       getDocs();

private:
    std::string            hid;
    std::string            name;
    std::string            loc;
    std::string            spec;
    int                    cap  = 0;
    std::vector<Doctor>    docs;
    std::vector<std::string> pids;
};
