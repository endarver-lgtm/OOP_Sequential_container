#pragma once
#include <array>
#include <list>
#include <string>
#include <vector>
#include "Equipment.h"
#include "Operator.h"
#include "WorkAssignment.h"

class CompanyFleet {
private:
    std::vector<Equipment*> fleet_;        // Контейнер vector: динамический парк техники
    std::list<WorkAssignment> assignments_; // Контейнер list: журнал нарядов занятости техники
    std::array<Operator, 5> operators_;    // Контейнер array: фиксированный штат машинистов

    int nextEquipmentId_;
    int nextAssignmentId_;

public:
    CompanyFleet();
    ~CompanyFleet();

    void seedDemoData();

    // --- Операции с вектором (парк техники) ---
    void addExcavator(const std::string& model, const std::string& regNum,
                      double rate, const std::string& status, double bucket, Operator* op);
    void addCrane(const std::string& model, const std::string& regNum,
                  double rate, const std::string& status, double maxCap, Operator* op);
    bool removeEquipment(int id);
    Equipment* findEquipmentById(int id);
    std::vector<Equipment*> findEquipmentByQuery(const std::string& query) const;
    std::vector<Equipment*> filterEquipmentByType(const std::string& type) const;
    void sortFleetByRate(bool ascending = true);
    void sortFleetByModel();
    void printFleet(std::ostream& os) const;
    void exportFleetToFile(const std::string& filename) const;
    std::vector<Equipment*>& fleet() { return fleet_; }

    // --- Операции со списком (журнал нарядов) ---
    void addAssignment(const std::string& siteName, Equipment* eq, const WorkPeriod& period);
    bool removeAssignment(int id);
    WorkAssignment* findAssignmentById(int id);
    std::vector<WorkAssignment*> findAssignmentsBySite(const std::string& site) ;
    void sortAssignmentsByCost(bool ascending = false);
    void sortAssignmentsById();
    void printAssignments(std::ostream& os) const;
    void exportAssignmentsToFile(const std::string& filename) const;
    std::list<WorkAssignment>& assignments() { return assignments_; }

    // --- Операции с массивом (штат машинистов) ---
    bool setOperator(int slotIndex, const Operator& op);
    bool clearOperator(int slotIndex);
    Operator* findOperatorById(int id);
    Operator* findOperatorByName(const std::string& name);
    void sortOperatorsByExperience(bool ascending = false);
    void printOperators(std::ostream& os) const;
    void exportOperatorsToFile(const std::string& filename) const;
    std::array<Operator, 5>& operators() { return operators_; }

    // --- Сводный вывод и экспорт ---
    void printAll(std::ostream& os) const;
    void exportAllToFile(const std::string& filename) const;
};
