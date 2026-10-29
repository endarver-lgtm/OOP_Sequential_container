#pragma once
#include <iostream>
#include <string>
#include "Equipment.h"

// Класс периода работ (используется в составе наряда через композицию)
class WorkPeriod {
private:
    std::string startDate_;
    int days_;
    int hoursPerDay_;

public:
    WorkPeriod();
    WorkPeriod(const std::string& startDate, int days, int hoursPerDay);

    const std::string& startDate() const { return startDate_; }
    int days() const { return days_; }
    int hoursPerDay() const { return hoursPerDay_; }

    void setStartDate(const std::string& d) { startDate_ = d; }
    void setDays(int days) { days_ = days; }
    void setHoursPerDay(int h) { hoursPerDay_ = h; }

    int totalHours() const { return days_ * hoursPerDay_; }
};

// Класс наряда на занятость строительной техники
class WorkAssignment {
private:
    int id_;
    std::string siteName_;
    Equipment* equipment_;   // Агрегация: связь с объектом техники
    WorkPeriod period_;      // Композиция: период создаётся и уничтожается вместе с нарядом

public:
    WorkAssignment();
    WorkAssignment(int id, const std::string& siteName, Equipment* eq, const WorkPeriod& period);

    int id() const { return id_; }
    const std::string& siteName() const { return siteName_; }
    Equipment* equipment() const { return equipment_; }
    const WorkPeriod& period() const { return period_; }
    WorkPeriod& period() { return period_; }

    void setId(int id) { id_ = id; }
    void setSiteName(const std::string& s) { siteName_ = s; }
    void setEquipment(Equipment* eq) { equipment_ = eq; }
    void setPeriod(const WorkPeriod& p) { period_ = p; }

    double calculateTotalCost() const;

    static void printHeader(std::ostream& os);
    void printRow(std::ostream& os) const;
};
