#include "WorkAssignment.h"
#include <iomanip>

WorkPeriod::WorkPeriod()
    : startDate_("2026-01-01"), days_(1), hoursPerDay_(8) {
}

WorkPeriod::WorkPeriod(const std::string& startDate, int days, int hoursPerDay)
    : startDate_(startDate), days_(days), hoursPerDay_(hoursPerDay) {
}

WorkAssignment::WorkAssignment()
    : id_(0), siteName_("—"), equipment_(nullptr), period_() {
}

WorkAssignment::WorkAssignment(int id, const std::string& siteName, Equipment* eq, const WorkPeriod& period)
    : id_(id), siteName_(siteName), equipment_(eq), period_(period) {
}

double WorkAssignment::calculateTotalCost() const {
    if (!equipment_) return 0.0;
    return equipment_->hourlyRate() * period_.totalHours();
}

void WorkAssignment::printHeader(std::ostream& os) {
    std::ios::fmtflags old = os.flags();
    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << "№"
       << std::setw(24) << "Строительный объект"
       << std::setw(20) << "Техника"
       << std::setw(14) << "Гос. номер"
       << std::setw(12) << "Дата старта"
       << std::setw(8)  << "Дней"
       << std::setw(8)  << "Ч/день"
       << std::setw(10) << "Всего ч."
       << std::setw(14) << "Итого, руб."
       << "\n";
    os << std::string(115, '-') << "\n";
    os.flags(old);
}

void WorkAssignment::printRow(std::ostream& os) const {
    std::ios::fmtflags old = os.flags();
    std::streamsize oldPrec = os.precision();

    os.setf(std::ios::left, std::ios::adjustfield);
    os << std::setw(5)  << id_
       << std::setw(24) << siteName_
       << std::setw(20) << (equipment_ ? equipment_->model() : "Не указана")
       << std::setw(14) << (equipment_ ? equipment_->regNumber() : "—")
       << std::setw(12) << period_.startDate()
       << std::setw(8)  << period_.days()
       << std::setw(8)  << period_.hoursPerDay()
       << std::setw(10) << period_.totalHours();

    os.setf(std::ios::fixed, std::ios::floatfield);
    os << std::setw(14) << std::setprecision(2) << calculateTotalCost()
       << "\n";

    os.flags(old);
    os.precision(oldPrec);
}
