#pragma once
#include <iostream>
#include <string>

class Operator {
private:
    int id_;
    std::string fullName_;
    int experienceYears_;
    std::string category_;
    std::string phone_;

public:
    Operator();
    Operator(int id, const std::string& fullName, int experienceYears,
             const std::string& category, const std::string& phone);

    int id() const { return id_; }
    const std::string& fullName() const { return fullName_; }
    int experienceYears() const { return experienceYears_; }
    const std::string& category() const { return category_; }
    const std::string& phone() const { return phone_; }

    void setId(int id) { id_ = id; }
    void setFullName(const std::string& name) { fullName_ = name; }
    void setExperienceYears(int years) { experienceYears_ = years; }
    void setCategory(const std::string& cat) { category_ = cat; }
    void setPhone(const std::string& ph) { phone_ = ph; }

    bool isEmpty() const { return id_ == 0; }
    void clear();

    static void printHeader(std::ostream& os);
    void printRow(std::ostream& os) const;
};
