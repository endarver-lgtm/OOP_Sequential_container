#include "CompanyFleet.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

static std::string toLowerStr(std::string s) {
    for (char& c : s) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return s;
}

CompanyFleet::CompanyFleet()
    : nextEquipmentId_(1), nextAssignmentId_(1) {
}

CompanyFleet::~CompanyFleet() {
    for (auto eq : fleet_) delete eq;
    fleet_.clear();
}

void CompanyFleet::seedDemoData() {
    // 1. Заполняем штат машинистов (std::array на 5 мест)
    operators_[0] = Operator(101, "Иванов Иван Сергеевич",   12, "6-й разряд", "+375 29 111-22-33");
    operators_[1] = Operator(102, "Петров Алексей Павлович", 8,  "5-й разряд", "+375 29 222-33-44");
    operators_[2] = Operator(103, "Сидоров Дмитрий Олегович",15, "Машинист 1 кл", "+375 29 333-44-55");
    operators_[3] = Operator(104, "Ковалёв Роман Игоревич",  5,  "4-й разряд", "+375 29 444-55-66");
    operators_[4] = Operator(105, "Морозов Денис Васильевич",3,  "4-й разряд", "+375 29 555-66-77");

    // 2. Заполняем парк строительной техники (std::vector)
    addExcavator("Caterpillar 320D", "5432 AB-7", 130.00, "В работе", 1.20, &operators_[0]);
    addExcavator("Komatsu PC210",    "1122 BK-7", 115.50, "В работе", 1.00, &operators_[1]);
    addExcavator("JCB JS220",        "8899 MH-7", 125.00, "Свободен", 1.10, nullptr);
    addCrane("Liebherr LTM 1050",   "7700 TT-7", 180.00, "В работе", 50.0, &operators_[2]);
    addCrane("Ивановец КС-45717",   "3344 AX-7", 95.00,  "Свободен", 25.0, &operators_[3]);
    addCrane("Zoomlion ZTC600",     "9012 EE-7", 210.00, "На ТО",     60.0, nullptr);

    // 3. Заполняем журнал нарядов занятости техники (std::list)
    addAssignment("ЖК 'Минск-Мир'",  fleet_[0], WorkPeriod("2026-11-01", 10, 8));
    addAssignment("Мост через Немигу", fleet_[1], WorkPeriod("2026-11-03", 5, 10));
    addAssignment("ТЦ 'Северный'",    fleet_[3], WorkPeriod("2026-11-02", 7, 8));
}

// ======================= Операции с VECTOR =======================

void CompanyFleet::addExcavator(const std::string& model, const std::string& regNum,
                                double rate, const std::string& status, double bucket, Operator* op) {
    Equipment* eq = new Excavator(nextEquipmentId_++, model, regNum, rate, status, bucket, op);
    fleet_.push_back(eq);
}

void CompanyFleet::addCrane(const std::string& model, const std::string& regNum,
                            double rate, const std::string& status, double maxCap, Operator* op) {
    Equipment* eq = new Crane(nextEquipmentId_++, model, regNum, rate, status, maxCap, op);
    fleet_.push_back(eq);
}

bool CompanyFleet::removeEquipment(int id) {
    auto it = std::find_if(fleet_.begin(), fleet_.end(),
                           [id](Equipment* eq) { return eq->id() == id; });
    if (it == fleet_.end()) return false;

    Equipment* target = *it;
    // Очищаем ссылку на эту технику в нарядах
    for (auto& a : assignments_) {
        if (a.equipment() == target) a.setEquipment(nullptr);
    }

    delete target;
    fleet_.erase(it);
    return true;
}

Equipment* CompanyFleet::findEquipmentById(int id) {
    for (auto eq : fleet_) if (eq->id() == id) return eq;
    return nullptr;
}

std::vector<Equipment*> CompanyFleet::findEquipmentByQuery(const std::string& query) const {
    std::vector<Equipment*> res;
    std::string q = toLowerStr(query);
    for (auto eq : fleet_) {
        if (toLowerStr(eq->model()).find(q) != std::string::npos ||
            toLowerStr(eq->regNumber()).find(q) != std::string::npos) {
            res.push_back(eq);
        }
    }
    return res;
}

std::vector<Equipment*> CompanyFleet::filterEquipmentByType(const std::string& type) const {
    std::vector<Equipment*> res;
    std::string t = toLowerStr(type);
    for (auto eq : fleet_) {
        if (toLowerStr(eq->typeName()).find(t) != std::string::npos) {
            res.push_back(eq);
        }
    }
    return res;
}

void CompanyFleet::sortFleetByRate(bool ascending) {
    std::sort(fleet_.begin(), fleet_.end(), [ascending](Equipment* a, Equipment* b) {
        return ascending ? (a->hourlyRate() < b->hourlyRate())
                         : (a->hourlyRate() > b->hourlyRate());
    });
}

void CompanyFleet::sortFleetByModel() {
    std::sort(fleet_.begin(), fleet_.end(), [](Equipment* a, Equipment* b) {
        return a->model() < b->model();
    });
}

void CompanyFleet::printFleet(std::ostream& os) const {
    os << "\n=== ПАРК СТРОИТЕЛЬНОЙ ТЕХНИКИ (std::vector, позиций: " << fleet_.size() << ") ===\n";
    if (fleet_.empty()) {
        os << "(парк техники пуст)\n";
        return;
    }
    Equipment::printHeader(os);
    for (auto eq : fleet_) eq->printRow(os);
}

void CompanyFleet::exportFleetToFile(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    printFleet(f);
    std::cout << "Данные парка техники успешно сохранены в файл: " << filename << "\n";
}

// ======================= Операции с LIST =======================

void CompanyFleet::addAssignment(const std::string& siteName, Equipment* eq, const WorkPeriod& period) {
    assignments_.push_back(WorkAssignment(nextAssignmentId_++, siteName, eq, period));
    if (eq && eq->status() != "На ТО") {
        eq->setStatus("В работе");
    }
}

bool CompanyFleet::removeAssignment(int id) {
    auto it = std::find_if(assignments_.begin(), assignments_.end(),
                           [id](const WorkAssignment& a) { return a.id() == id; });
    if (it == assignments_.end()) return false;
    assignments_.erase(it);
    return true;
}

WorkAssignment* CompanyFleet::findAssignmentById(int id) {
    for (auto& a : assignments_) {
        if (a.id() == id) return &a;
    }
    return nullptr;
}

std::vector<WorkAssignment*> CompanyFleet::findAssignmentsBySite(const std::string& site) {
    std::vector<WorkAssignment*> res;
    std::string s = toLowerStr(site);
    for (auto& a : assignments_) {
        if (toLowerStr(a.siteName()).find(s) != std::string::npos) {
            res.push_back(&a);
        }
    }
    return res;
}

void CompanyFleet::sortAssignmentsByCost(bool ascending) {
    assignments_.sort([ascending](const WorkAssignment& a, const WorkAssignment& b) {
        return ascending ? (a.calculateTotalCost() < b.calculateTotalCost())
                         : (a.calculateTotalCost() > b.calculateTotalCost());
    });
}

void CompanyFleet::sortAssignmentsById() {
    assignments_.sort([](const WorkAssignment& a, const WorkAssignment& b) {
        return a.id() < b.id();
    });
}

void CompanyFleet::printAssignments(std::ostream& os) const {
    os << "\n=== ЖУРНАЛ НАРЯДОВ ЗАНЯТОСТИ ТЕХНИКИ (std::list, записей: " << assignments_.size() << ") ===\n";
    if (assignments_.empty()) {
        os << "(журнал нарядов пуст)\n";
        return;
    }
    WorkAssignment::printHeader(os);
    for (const auto& a : assignments_) a.printRow(os);
}

void CompanyFleet::exportAssignmentsToFile(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    printAssignments(f);
    std::cout << "Журнал нарядов успешно сохранён в файл: " << filename << "\n";
}

// ======================= Операции с ARRAY =======================

bool CompanyFleet::setOperator(int slotIndex, const Operator& op) {
    if (slotIndex < 0 || slotIndex >= static_cast<int>(operators_.size())) return false;
    operators_[slotIndex] = op;
    return true;
}

bool CompanyFleet::clearOperator(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= static_cast<int>(operators_.size())) return false;
    Operator* opPtr = &operators_[slotIndex];
    // Если машинист был прикреплен к технике, снимаем его
    for (auto eq : fleet_) {
        if (eq->assignedOperator() == opPtr) eq->setAssignedOperator(nullptr);
    }
    operators_[slotIndex].clear();
    return true;
}

Operator* CompanyFleet::findOperatorById(int id) {
    for (auto& op : operators_) {
        if (!op.isEmpty() && op.id() == id) return &op;
    }
    return nullptr;
}

Operator* CompanyFleet::findOperatorByName(const std::string& name) {
    std::string n = toLowerStr(name);
    for (auto& op : operators_) {
        if (!op.isEmpty() && toLowerStr(op.fullName()).find(n) != std::string::npos) {
            return &op;
        }
    }
    return nullptr;
}

void CompanyFleet::sortOperatorsByExperience(bool ascending) {
    std::sort(operators_.begin(), operators_.end(), [ascending](const Operator& a, const Operator& b) {
        return ascending ? (a.experienceYears() < b.experienceYears())
                         : (a.experienceYears() > b.experienceYears());
    });
}

void CompanyFleet::printOperators(std::ostream& os) const {
    os << "\n=== ШТАТ МАШИНИСТОВ (std::array<Operator, 5>) ===\n";
    Operator::printHeader(os);
    for (const auto& op : operators_) op.printRow(os);
}

void CompanyFleet::exportOperatorsToFile(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    printOperators(f);
    std::cout << "Штат машинистов успешно сохранён в файл: " << filename << "\n";
}

// ======================= Сводный вывод и экспорт =======================

void CompanyFleet::printAll(std::ostream& os) const {
    os << "===================================================================================\n";
    os << "        СИСТЕМА УЧЕТА ЗАНЯТОСТИ СТРОИТЕЛЬНОЙ ТЕХНИКИ В СТРОИТЕЛЬНОЙ КОМПАНИИ       \n";
    os << "===================================================================================\n";
    printFleet(os);
    printAssignments(os);
    printOperators(os);
}

void CompanyFleet::exportAllToFile(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи\n";
        return;
    }
    printAll(f);
    std::cout << "Сводная таблица успешно экспортирована в файл: " << filename << "\n";
}
