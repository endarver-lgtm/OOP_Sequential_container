#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include "CompanyFleet.h"
#include "Input.h"

using namespace std;

// Меню работы с контейнером std::vector (парк техники)
static void menuVector(CompanyFleet& fleet) {
    while (true) {
        clearScreen();
        cout << "--- РАБОТА С ПАРКОМ ТЕХНИКИ (std::vector) ---\n"
             << "1. Просмотр парка техники на экран\n"
             << "2. Экспорт парка техники в файл\n"
             << "3. Добавить экскаватор\n"
             << "4. Добавить автокран\n"
             << "5. Редактировать технику (тариф, статус, машинист)\n"
             << "6. Удалить технику по ID\n"
             << "7. Поиск техники (по модели или гос. номеру)\n"
             << "8. Фильтр техники по типу (экскаватор / кран)\n"
             << "9. Сортировка по тарифу (по возрастанию / убыванию)\n"
             << "10. Сортировка по названию модели (по алфавиту)\n"
             << "0. Назад в главное меню\n";

        int choice = readInt(cin, "Ваш выбор: ", 0, 10);
        clearScreen();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            fleet.printFleet(cout);
            break;
        case 2: {
            string filename = readLine(cin, "Введите имя файла (напр. fleet.txt): ", 50);
            fleet.exportFleetToFile(filename);
            break;
        }
        case 3: {
            cout << "Добавление экскаватора:\n";
            string model = readLine(cin, "Модель (напр. Hyundai R220): ", 30);
            string reg = readLine(cin, "Гос. номер (напр. 4455 AB-7): ", 20);
            double rate = readDouble(cin, "Тариф за час, руб. (1-9999): ", 1.0, 9999.0);
            double bucket = readDouble(cin, "Объем ковша, м3 (0.1-15.0): ", 0.1, 15.0);

            fleet.printOperators(cout);
            int opId = readInt(cin, "ID машиниста для прикрепления (0 - без машиниста): ", 0, 999);
            Operator* op = (opId == 0) ? nullptr : fleet.findOperatorById(opId);

            fleet.addExcavator(model, reg, rate, "Свободен", bucket, op);
            cout << "Экскаватор успешно добавлен в парк!\n";
            break;
        }
        case 4: {
            cout << "Добавление автокрана:\n";
            string model = readLine(cin, "Модель (напр. Клинцы КС-55713): ", 30);
            string reg = readLine(cin, "Гос. номер (напр. 6677 MH-7): ", 20);
            double rate = readDouble(cin, "Тариф за час, руб. (1-9999): ", 1.0, 9999.0);
            double cap = readDouble(cin, "Грузоподъемность, тонн (1-500): ", 1.0, 500.0);

            fleet.printOperators(cout);
            int opId = readInt(cin, "ID машиниста для прикрепления (0 - без машиниста): ", 0, 999);
            Operator* op = (opId == 0) ? nullptr : fleet.findOperatorById(opId);

            fleet.addCrane(model, reg, rate, "Свободен", cap, op);
            cout << "Автокран успешно добавлен в парк!\n";
            break;
        }
        case 5: {
            fleet.printFleet(cout);
            int id = readInt(cin, "Введите ID техники для редактирования: ", 1, 9999);
            Equipment* eq = fleet.findEquipmentById(id);
            if (!eq) {
                cout << "Техника с таким ID не найдена.\n";
                break;
            }
            cout << "Текущий тариф: " << eq->hourlyRate() << " руб/ч. Новый тариф: ";
            eq->setHourlyRate(readDouble(cin, "", 1.0, 9999.0));

            cout << "Выберите статус:\n 1 - Свободен\n 2 - В работе\n 3 - На ТО\n";
            int stChoice = readInt(cin, "Статус: ", 1, 3);
            if (stChoice == 1) eq->setStatus("Свободен");
            else if (stChoice == 2) eq->setStatus("В работе");
            else eq->setStatus("На ТО");

            fleet.printOperators(cout);
            int opId = readInt(cin, "ID машиниста (0 - снять назначение): ", 0, 999);
            eq->setAssignedOperator((opId == 0) ? nullptr : fleet.findOperatorById(opId));

            cout << "Данные техники успешно обновлены!\n";
            break;
        }
        case 6: {
            fleet.printFleet(cout);
            int id = readInt(cin, "Введите ID техники для удаления: ", 1, 9999);
            if (askYesNo(cin, "Вы уверены, что хотите удалить эту технику? (y/n): ")) {
                if (fleet.removeEquipment(id)) cout << "Техника успешно удалена из парка.\n";
                else cout << "Техника с ID " << id << " не найдена.\n";
            } else {
                cout << "Удаление отменено.\n";
            }
            break;
        }
        case 7: {
            string q = readLine(cin, "Введите строку поиска (модель или номер): ", 30);
            auto results = fleet.findEquipmentByQuery(q);
            if (results.empty()) {
                cout << "По запросу \"" << q << "\" ничего не найдено.\n";
            } else {
                cout << "\nРезультаты поиска (" << results.size() << " ед.):\n";
                Equipment::printHeader(cout);
                for (auto eq : results) eq->printRow(cout);
            }
            break;
        }
        case 8: {
            cout << "1 - Экскаваторы\n2 - Автокраны\n";
            int typeChoice = readInt(cin, "Выберите тип: ", 1, 2);
            string filterType = (typeChoice == 1) ? "Экскаватор" : "Автокран";
            auto filtered = fleet.filterEquipmentByType(filterType);
            cout << "\nОтфильтрованная техника (" << filtered.size() << " ед.):\n";
            Equipment::printHeader(cout);
            for (auto eq : filtered) eq->printRow(cout);
            break;
        }
        case 9: {
            cout << "1 - По возрастанию цены\n2 - По убыванию цены\n";
            int dir = readInt(cin, "Выбор направления: ", 1, 2);
            fleet.sortFleetByRate(dir == 1);
            cout << "Парк техники отсортирован по тарифу.\n";
            fleet.printFleet(cout);
            break;
        }
        case 10:
            fleet.sortFleetByModel();
            cout << "Парк техники отсортирован по модели.\n";
            fleet.printFleet(cout);
            break;
        }

        pauseScreen();
    }
}

// Меню работы с контейнером std::list (журнал нарядов занятости техники)
static void menuList(CompanyFleet& fleet) {
    while (true) {
        clearScreen();
        cout << "--- РАБОТА С ЖУРНАЛОМ НАРЯДОВ ЗАНЯТОСТИ (std::list) ---\n"
             << "1. Просмотр всех нарядов на экран\n"
             << "2. Экспорт нарядов в файл\n"
             << "3. Создать новый наряд на занятость техники\n"
             << "4. Редактировать наряд (объект, дни, часы)\n"
             << "5. Удалить наряд по номеру\n"
             << "6. Поиск нарядов по названию стройобъекта\n"
             << "7. Сортировка нарядов по общей стоимости (по убыванию/возрастанию)\n"
             << "8. Сортировка нарядов по номеру ID\n"
             << "0. Назад в главное меню\n";

        int choice = readInt(cin, "Ваш выбор: ", 0, 8);
        clearScreen();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            fleet.printAssignments(cout);
            break;
        case 2: {
            string filename = readLine(cin, "Введите имя файла (напр. assignments.txt): ", 50);
            fleet.exportAssignmentsToFile(filename);
            break;
        }
        case 3: {
            cout << "Оформление наряда на занятость техники:\n";
            string site = readLine(cin, "Название стройобъекта (напр. ЖК Престиж): ", 40);

            fleet.printFleet(cout);
            int eqId = readInt(cin, "Введите ID выделяемой техники: ", 1, 9999);
            Equipment* eq = fleet.findEquipmentById(eqId);
            if (!eq) {
                cout << "Техника не найдена! Наряд не создан.\n";
                break;
            }

            string date = readLine(cin, "Дата начала работ (ГГГГ-ММ-ДД): ", 12);
            int days = readInt(cin, "Срок аренды (дней, 1-365): ", 1, 365);
            int hpd = readInt(cin, "Количество часов в смену (1-24): ", 1, 24);

            fleet.addAssignment(site, eq, WorkPeriod(date, days, hpd));
            cout << "Наряд успешно создан!\n";
            break;
        }
        case 4: {
            fleet.printAssignments(cout);
            int id = readInt(cin, "Введите номер наряда для редактирования: ", 1, 9999);
            WorkAssignment* a = fleet.findAssignmentById(id);
            if (!a) {
                cout << "Наряд с номером " << id << " не найден.\n";
                break;
            }

            cout << "Текущий объект: " << a->siteName() << "\n";
            a->setSiteName(readLine(cin, "Новое название объекта: ", 40));

            int days = readInt(cin, "Новое количество дней (1-365): ", 1, 365);
            int hpd = readInt(cin, "Новое кол-во часов в день (1-24): ", 1, 24);
            a->period().setDays(days);
            a->period().setHoursPerDay(hpd);

            cout << "Наряд обновлен. Новая итоговая стоимость: "
                 << a->calculateTotalCost() << " руб.\n";
            break;
        }
        case 5: {
            fleet.printAssignments(cout);
            int id = readInt(cin, "Введите номер наряда для удаления: ", 1, 9999);
            if (askYesNo(cin, "Удалить этот наряд? (y/n): ")) {
                if (fleet.removeAssignment(id)) cout << "Наряд успешно удален.\n";
                else cout << "Наряд с номером " << id << " не найден.\n";
            }
            break;
        }
        case 6: {
            string s = readLine(cin, "Введите название или часть названия объекта: ", 30);
            auto res = fleet.findAssignmentsBySite(s);
            if (res.empty()) {
                cout << "Нарядов на объекте \"" << s << "\" не найдено.\n";
            } else {
                cout << "\nНайденные наряды (" << res.size() << " шт.):\n";
                WorkAssignment::printHeader(cout);
                for (auto a : res) a->printRow(cout);
            }
            break;
        }
        case 7: {
            cout << "1 - От дорогих к дешевым (по убыванию)\n2 - От дешевых к дорогим (по возрастанию)\n";
            int d = readInt(cin, "Выбор: ", 1, 2);
            fleet.sortAssignmentsByCost(d == 2);
            cout << "Наряды отсортированы по стоимости.\n";
            fleet.printAssignments(cout);
            break;
        }
        case 8:
            fleet.sortAssignmentsById();
            cout << "Наряды отсортированы по номеру наряда (ID).\n";
            fleet.printAssignments(cout);
            break;
        }

        pauseScreen();
    }
}

// Меню работы с контейнером std::array (штат машинистов)
static void menuArray(CompanyFleet& fleet) {
    while (true) {
        clearScreen();
        cout << "--- РАБОТА СО ШТАТОМ МАШИНИСТОВ (std::array<Operator, 5>) ---\n"
             << "1. Просмотр штата машинистов на экран\n"
             << "2. Экспорт штата в файл\n"
             << "3. Назначить / редактировать машиниста в слоте (слоты 1-5)\n"
             << "4. Очистить слот машиниста\n"
             << "5. Поиск машиниста по имени\n"
             << "6. Сортировка машинистов по стажу работы\n"
             << "0. Назад в главное меню\n";

        int choice = readInt(cin, "Ваш выбор: ", 0, 6);
        clearScreen();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            fleet.printOperators(cout);
            break;
        case 2: {
            string filename = readLine(cin, "Введите имя файла (напр. operators.txt): ", 50);
            fleet.exportOperatorsToFile(filename);
            break;
        }
        case 3: {
            fleet.printOperators(cout);
            int slot = readInt(cin, "Выберите номер слота для записи (1-5): ", 1, 5);
            int id = readInt(cin, "ID машиниста: ", 1, 9999);
            string name = readLine(cin, "ФИО машиниста: ", 30);
            int exp = readInt(cin, "Стаж работы (лет, 0-50): ", 0, 50);
            string cat = readLine(cin, "Разряд/категория (напр. 6-й разряд): ", 20);
            string phone = readLine(cin, "Телефон: ", 20);

            fleet.setOperator(slot - 1, Operator(id, name, exp, cat, phone));
            cout << "Слот " << slot << " успешно обновлен!\n";
            break;
        }
        case 4: {
            fleet.printOperators(cout);
            int slot = readInt(cin, "Выберите номер слота для очистки (1-5): ", 1, 5);
            if (askYesNo(cin, "Очистить этот слот? (y/n): ")) {
                fleet.clearOperator(slot - 1);
                cout << "Слот " << slot << " очищен.\n";
            }
            break;
        }
        case 5: {
            string name = readLine(cin, "Введите имя или фамилию для поиска: ", 30);
            Operator* op = fleet.findOperatorByName(name);
            if (op) {
                cout << "\nНайден машинист:\n";
                Operator::printHeader(cout);
                op->printRow(cout);
            } else {
                cout << "Машинист с таким именем не найден.\n";
            }
            break;
        }
        case 6: {
            cout << "1 - По убыванию стажа (от опытных)\n2 - По возрастанию стажа\n";
            int d = readInt(cin, "Выбор: ", 1, 2);
            fleet.sortOperatorsByExperience(d == 2);
            cout << "Штат машинистов отсортирован по стажу.\n";
            fleet.printOperators(cout);
            break;
        }
        }

        pauseScreen();
    }
}

int main() {
    // Настройка консоли на русский язык
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    CompanyFleet fleet;
    fleet.seedDemoData();

    while (true) {
        clearScreen();
        cout << "===================================================================================\n"
             << "   СИСТЕМА УЧЕТА ЗАНЯТОСТИ СТРОИТЕЛЬНОЙ ТЕХНИКИ (ЛР №3, Вариант 10)               \n"
             << "===================================================================================\n"
             << "1. Сводный просмотр всех данных (парк, наряды, машинисты)\n"
             << "2. Экспорт всей сводной таблицы в файл\n"
             << "3. Управление парком техники (контейнер std::vector)\n"
             << "4. Управление журналом нарядов (контейнер std::list)\n"
             << "5. Управление штатом машинистов (контейнер std::array)\n"
             << "0. Выход из программы\n"
             << "-----------------------------------------------------------------------------------\n";

        int choice = readInt(cin, "Выберите пункт меню: ", 0, 5);
        clearScreen();

        switch (choice) {
        case 1:
            fleet.printAll(cout);
            pauseScreen();
            break;
        case 2: {
            string filename = readLine(cin, "Введите имя файла (напр. all_data.txt): ", 50);
            fleet.exportAllToFile(filename);
            pauseScreen();
            break;
        }
        case 3:
            menuVector(fleet);
            break;
        case 4:
            menuList(fleet);
            break;
        case 5:
            menuArray(fleet);
            break;
        case 0:
            if (askYesNo(cin, "Вы действительно хотите выйти из программы? (y/n): ")) {
                cout << "\nПрограмма завершена. До свидания!\n";
                return 0;
            }
            break;
        }
    }

    return 0;
}
