#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <Windows.h>

using namespace std;

// Структура для хранения информации о здании
struct Building {
    char street[50]; // Фиксированный массив символов для улицы
    int houseNumber;
    int apartmentsCount;
};

// Класс для работы с базой данных
class Database {
private:
    Building* buildings; // Динамический массив зданий
    int size;           // Текущий размер массива
    int capacity;       // Вместимость массива

    // Увеличение размера массива
    void resize() {
        capacity *= 2;
        Building* newBuildings = new Building[capacity];
        for (int i = 0; i < size; ++i) {
            newBuildings[i] = buildings[i];
        }
        delete[] buildings;
        buildings = newBuildings;
    }

public:
    // Конструктор
    Database() : size(0), capacity(10) {
        buildings = new Building[capacity];
    }

    // Деструктор
    ~Database() {
        delete[] buildings;
    }

    // Добавление записи
    void addBuilding(const string& street, int houseNumber, int apartmentsCount) {
        if (size >= capacity) {
            resize();
        }
        strncpy_s(buildings[size].street, street.c_str(), 49); // Копируем строку в массив char
        buildings[size].street[49] = '\0'; // Убедимся, что строка завершается нулевым символом
        buildings[size].houseNumber = houseNumber;
        buildings[size].apartmentsCount = apartmentsCount;
        size++;
    }

    // Удаление записи по индексу
    void deleteBuilding(int index) {
        if (index < 0 || index >= size) {
            cout << "Ошибка: неверный индекс.\n";
            return;
        }
        for (int i = index; i < size - 1; ++i) {
            buildings[i] = buildings[i + 1];
        }
        size--;
    }

    // Вывод всех записей в виде таблицы
    void printBuildings() const {
        cout << "\nУлица\t\tНомер дома\tКоличество квартир\n";
        for (int i = 0; i < size; ++i) {
            cout << buildings[i].street << "\t\t" << buildings[i].houseNumber << "\t\t" << buildings[i].apartmentsCount << "\n";
        }
    }

    // Расчет общего количества квартир
    int calculateTotalApartments() const {
        int total = 0;
        for (int i = 0; i < size; ++i) {
            total += buildings[i].apartmentsCount;
        }
        return total;
    }

    // Чтение данных из текстового файла
    void readFromTextFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << "\n";
            return;
        }
        size = 0; // Сброс текущего размера массива
        string street;
        int houseNumber, apartmentsCount;
        while (file >> street >> houseNumber >> apartmentsCount) {
            addBuilding(street, houseNumber, apartmentsCount);
        }
        file.close();
    }

    // Чтение данных из двоичного файла
    void readFromBinaryFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << "\n";
            return;
        }
        size = 0; // Сброс текущего размера массива
        Building temp;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Building))) {
            addBuilding(temp.street, temp.houseNumber, temp.apartmentsCount);
        }
        file.close();
    }

    // Запись данных в текстовый файл
    void writeToTextFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << "\n";
            return;
        }
        for (int i = 0; i < size; ++i) {
            file << buildings[i].street << " " << buildings[i].houseNumber << " " << buildings[i].apartmentsCount << "\n";
        }
        file.close();
    }

    // Запись данных в двоичный файл
    void writeToBinaryFile(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << "\n";
            return;
        }
        for (int i = 0; i < size; ++i) {
            file.write(reinterpret_cast<const char*>(&buildings[i]), sizeof(Building));
        }
        file.close();
    }

    // Расчет размера файла
    long getFileSize(const string& filename) const {
        ifstream file(filename, ios::binary | ios::ate);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << "\n";
            return -1;
        }
        return file.tellg();
    }
};

// Функция для отображения меню
void printMenu() {
    cout << "\nМеню:\n";
    cout << "1. Ввод данных с клавиатуры\n";
    cout << "2. Чтение данных из текстового файла\n";
    cout << "3. Чтение данных из двоичного файла\n";
    cout << "4. Вывод данных на экран\n";
    cout << "5. Расчет общего количества квартир\n";
    cout << "6. Добавление записи\n";
    cout << "7. Удаление записи\n";
    cout << "8. Запись данных в текстовый файл\n";
    cout << "9. Запись данных в двоичный файл\n";
    cout << "10. Расчет размера файла\n";
    cout << "11. Выход\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Database db;
    int choice;
    string filename;
    string street;
    int houseNumber, apartmentsCount;

    do {
        printMenu();
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Введите улицу: ";
            cin >> street;
            cout << "Введите номер дома: ";
            cin >> houseNumber;
            cout << "Введите количество квартир: ";
            cin >> apartmentsCount;
            db.addBuilding(street, houseNumber, apartmentsCount);
            break;
        }
        case 2: {

            cout << "Введите имя текстового файла: ";
            cin >> filename;
            db.readFromTextFile(filename);
            break;
        }
        case 3:
        {
            cout << "Введите имя двоичного файла: ";
            cin >> filename;
            db.readFromBinaryFile(filename);
            break;
        }
        case 4:
        {
            db.printBuildings();
            break;
        }
        
        case 5:
        {
            cout << "Общее количество квартир: " << db.calculateTotalApartments() << "\n";
            break;
        }
        case 6:
        {
            cout << "Введите улицу: ";
            cin >> street;
            cout << "Введите номер дома: ";
            cin >> houseNumber;
            cout << "Введите количество квартир: ";
            cin >> apartmentsCount;
            db.addBuilding(street, houseNumber, apartmentsCount);
            break;
        }
        case 7:
        {
            cout << "Введите индекс записи для удаления: ";
            cin >> houseNumber;
            db.deleteBuilding(houseNumber - 1); // Индексация с 0
            break;
        }
        case 8:
        {
            cout << "Введите имя текстового файла: ";
            cin >> filename;
            db.writeToTextFile(filename);
            break;
        }
        case 9:
        {
            cout << "Введите имя двоичного файла: ";
            cin >> filename;
            db.writeToBinaryFile(filename);
            break;
        }
        case 10:
        {
            cout << "Введите имя файла: ";
            cin >> filename;
            long size = db.getFileSize(filename);
            if (size != -1) {
                cout << "Размер файла: " << size << " байт\n";
            }

            break;
        }
        case 11:
        {
            cout << "Выход из программы.\n";
            break;
        }
        default:
        {
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
        }
    } while (choice != 11);

    return 0;
}