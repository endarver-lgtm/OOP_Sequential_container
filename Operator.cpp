#include "Operator.h"
#include <iomanip>

Operator::Operator()
    : id_(0), fullName_("—"), experienceYears_(0), category_("—"), phone_("—") {
}

Operator::Operator(int id, const std::string& fullName, int experienceYears,
                   const std::string& category, const std::string& phone)
    : id_(id), fullName_(fullName), experienceYears_(experienceYears),
      category_(category), phone_(phone) {
}

void Operator::clear() {
    id_ = 0;
    fullName_ = "—";
    experienceYears_ = 0;
    category_ = "—";
    phone_ = "—";
}

void Operator::printHeader(std::ostream& os) {
    std::ios::fmtflags old = os.flags();
    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << "ID"
       << std::setw(26) << "ФИО машиниста"
       << std::setw(8)  << "Стаж"
       << std::setw(18) << "Разряд"
       << std::setw(16) << "Телефон"
       << "\n";
    os << std::string(73, '-') << "\n";
    os.flags(old);
}

void Operator::printRow(std::ostream& os) const {
    std::ios::fmtflags old = os.flags();
    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << id_
       << std::setw(26) << fullName_
       << std::setw(8)  << (std::to_string(experienceYears_) + " г.")
       << std::setw(18) << category_
       << std::setw(16) << phone_
       << "\n";
    os.flags(old);
}
