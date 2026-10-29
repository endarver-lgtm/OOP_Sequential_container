#pragma once
#include <iostream>
#include <string>
#include "Operator.h"

// Базовый класс строительной техники
class Equipment {
protected:
    int id_;
    std::string model_;
    std::string regNumber_;
    double hourlyRate_;
    std::string status_;
    Operator* assignedOperator_; // Агрегация: машинист существует независимо

public:
    Equipment();
    Equipment(int id, const std::string& model, const std::string& regNumber,
              double hourlyRate, const std::string& status, Operator* op = nullptr);
    virtual ~Equipment() {}

    int id() const { return id_; }
    const std::string& model() const { return model_; }
    const std::string& regNumber() const { return regNumber_; }
    double hourlyRate() const { return hourlyRate_; }
    const std::string& status() const { return status_; }
    Operator* assignedOperator() const { return assignedOperator_; }

    void setId(int id) { id_ = id; }
    void setModel(const std::string& m) { model_ = m; }
    void setRegNumber(const std::string& reg) { regNumber_ = reg; }
    void setHourlyRate(double rate) { hourlyRate_ = rate; }
    void setStatus(const std::string& st) { status_ = st; }
    void setAssignedOperator(Operator* op) { assignedOperator_ = op; }

    virtual std::string typeName() const = 0;
    virtual std::string getSpecs() const = 0;

    static void printHeader(std::ostream& os);
    virtual void printRow(std::ostream& os) const;
};

// Экскаватор (наследование от Equipment)
class Excavator : public Equipment {
private:
    double bucketCapacity_; // Объем ковша (м3)

public:
    Excavator();
    Excavator(int id, const std::string& model, const std::string& regNumber,
              double hourlyRate, const std::string& status,
              double bucketCapacity, Operator* op = nullptr);

    double bucketCapacity() const { return bucketCapacity_; }
    void setBucketCapacity(double cap) { bucketCapacity_ = cap; }

    std::string typeName() const override { return "Экскаватор"; }
    std::string getSpecs() const override;
};

// Подъемный кран (наследование от Equipment)
class Crane : public Equipment {
private:
    double maxCapacity_; // Грузоподъемность (тонн)

public:
    Crane();
    Crane(int id, const std::string& model, const std::string& regNumber,
          double hourlyRate, const std::string& status,
          double maxCapacity, Operator* op = nullptr);

    double maxCapacity() const { return maxCapacity_; }
    void setMaxCapacity(double cap) { maxCapacity_ = cap; }

    std::string typeName() const override { return "Автокран"; }
    std::string getSpecs() const override;
};
