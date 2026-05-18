#pragma once
#include <string>

// Abstract base class representing a person in the hospital system.
// Demonstrates: Abstraction (pure virtual displayInfo), Encapsulation (protected fields)
class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
    virtual ~Person() = default;

    // Pure virtual — forces subclasses to provide their own display logic
    virtual void displayInfo() const = 0;

    std::string getName() const { return name; }
    int getAge() const { return age; }
};
