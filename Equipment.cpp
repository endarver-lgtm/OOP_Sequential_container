#include "Equipment.h"
#include <iomanip>
#include <sstream>

Equipment::Equipment()
    : id_(0), model_("—"), regNumber_("—"), hourlyRate_(0.0),
      status_("Свободен"), assignedOperator_(nullptr) {
}

Equipment::Equipment(int id, const std::string& model, const std::string& regNumber,
                     double hourlyRate, const std::string& status, Operator* op)
    : id_(id), model_(model), regNumber_(regNumber),
      hourlyRate_(hourlyRate), status_(status), assignedOperator_(op) {
}

void Equipment::printHeader(std::ostream& os) {
    std::ios::fmtflags old = os.flags();
    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << "ID"
       << std::setw(12) << "Тип"
       << std::setw(22) << "Модель"
       << std::setw(14) << "Гос. номер"
       << std::setw(14) << "Тариф, руб/ч"
       << std::setw(12) << "Статус"
       << std::setw(16) << "Характеристики"
       << std::setw(24) << "Машинист"
       << "\n";
    os << std::string(119, '-') << "\n";
    os.flags(old);
}

void Equipment::printRow(std::ostream& os) const {
    std::ios::fmtflags old = os.flags();
    std::streamsize oldPrec = os.precision();

    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << id_
       << std::setw(12) << typeName()
       << std::setw(22) << model_
       << std::setw(14) << regNumber_;

    os.setf(std::ios::fixed, std::ios::floatfield);
    os << std::setw(14) << std::setprecision(2) << hourlyRate_;

    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(12) << status_
       << std::setw(16) << getSpecs()
       << std::setw(24) << (assignedOperator_ ? assignedOperator_->fullName() : "Не назначен")
       << "\n";

    os.flags(old);
    os.precision(oldPrec);
}

Excavator::Excavator()
    : Equipment(), bucketCapacity_(0.0) {
}

Excavator::Excavator(int id, const std::string& model, const std::string& regNumber,
                     double hourlyRate, const std::string& status,
                     double bucketCapacity, Operator* op)
    : Equipment(id, model, regNumber, hourlyRate, status, op),
      bucketCapacity_(bucketCapacity) {
}

std::string Excavator::getSpecs() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << bucketCapacity_ << " м3";
    return ss.str();
}

Crane::Crane()
    : Equipment(), maxCapacity_(0.0) {
}

Crane::Crane(int id, const std::string& model, const std::string& regNumber,
             double hourlyRate, const std::string& status,
             double maxCapacity, Operator* op)
    : Equipment(id, model, regNumber, hourlyRate, status, op),
      maxCapacity_(maxCapacity) {
}

std::string Crane::getSpecs() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << maxCapacity_ << " т";
    return ss.str();
}
