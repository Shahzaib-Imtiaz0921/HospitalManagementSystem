#pragma once
#include "person.h"
#include <string>

// Patient inherits from Person.
// Demonstrates: Inheritance, Encapsulation, Polymorphism (displayInfo override)
class Patient : public Person {
public:
    // Default constructor (needed for container storage)
    Patient() = default;

    // Primary constructor — if pid is empty, auto-generates "PAT-XXX"
    Patient(const std::string& name, int age,
            const std::string& loc, const std::string& sym,
            const std::string& pid = "");

    // Constructor used when loading from DB (pid already known)
    Patient(const std::string& pid, const std::string& name, int age,
            const std::string& loc, const std::string& sym);

    ~Patient() override = default;

    void displayInfo() const override;

    std::string getPID()  const;
    std::string getLoc()  const;
    std::string getSym()  const;
    bool        isDischarged() const;
    bool        isDone()  const;   // alias for isDischarged()
    std::string getHID()  const;
    std::string getDID()  const;

    void setHID(const std::string& hid);
    void setDID(const std::string& did);
    void discharge();

private:
    std::string pid;
    std::string loc;   // patient's sector / location
    std::string sym;   // symptoms
    std::string hid;   // assigned hospital ID
    std::string did;   // assigned doctor ID
    bool        disc = false;

    // Auto-ID counter shared across all Patient instances
    static int s_counter;
};
