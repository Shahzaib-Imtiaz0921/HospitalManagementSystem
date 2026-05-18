#pragma once
#include "person.h"
#include <string>
#include <utility>  // std::pair

// Doctor inherits from Person.
// Demonstrates: Inheritance, Encapsulation, Polymorphism (displayInfo override)
class Doctor : public Person {
public:
    // Default constructor (needed for container storage)
    Doctor() = default;

    // Primary constructor — if id is empty, auto-generates "DOC-XXX"
    Doctor(const std::string& name, int age,
           const std::string& spec, const std::string& hospitalId,
           const std::string& id = "");

    // Constructor used when loading from DB (id already known)
    Doctor(const std::string& id, const std::string& name, int age,
           const std::string& spec, const std::string& hospitalId);

    ~Doctor() override = default;

    void displayInfo() const override;

    std::string getID()         const;
    std::string getSpec()       const;
    bool        isFree()        const;
    void        setFree(bool v);
    std::string getHospitalId() const;
    void        setHospitalId(const std::string& hid);

    // Returns {diagnosis, medicines} based on actual symptoms text + specialization
    std::pair<std::string, std::string> diagnose(const std::string& symptoms) const;

private:
    std::string id;
    std::string spec;
    bool        avail      = true;
    std::string hospitalId;

    // Auto-ID counter shared across all Doctor instances
    static int s_counter;
};
