#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <Windows.h>

using namespace std;

// ��������� ��� �������� ���������� � ������
struct Building {
    char street[50]; // ������������� ������ �������� ��� �����
    int houseNumber;
    int apartmentsCount;
};

// ����� ��� ������ � ����� ������
class Database {
private:
    Building* buildings; // ������������ ������ ������
    int size;           // ������� ������ �������
    int capacity;       // ����������� �������

    // ���������� ������� �������
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
    // �����������
    Database() : size(0), capacity(10) {
        buildings = new Building[capacity];
    }

    // ����������
    ~Database() {
        delete[] buildings;
    }

    // ���������� ������
    void addBuilding(const string& street, int houseNumber, int apartmentsCount) {
        if (size >= capacity) {
            resize();
        }
        strncpy_s(buildings[size].street, street.c_str(), 49); // �������� ������ � ������ char
        buildings[size].street[49] = '\0'; // ��������, ��� ������ ����������� ������� ��������
        buildings[size].houseNumber = houseNumber;
        buildings[size].apartmentsCount = apartmentsCount;
        size++;
    }

    // �������� ������ �� �������
    void deleteBuilding(int index) {
        if (index < 0 || index >= size) {
            cout << "������: �������� ������.\n";
            return;
        }
        for (int i = index; i < size - 1; ++i) {
            buildings[i] = buildings[i + 1];
        }
        size--;
    }

    // ����� ���� ������� � ���� �������
    void printBuildings() const {
        cout << "\n�����\t\t����� ����\t���������� �������\n";
        for (int i = 0; i < size; ++i) {
            cout << buildings[i].street << "\t\t" << buildings[i].houseNumber << "\t\t" << buildings[i].apartmentsCount << "\n";
        }
    }

    // ������ ������ ���������� �������
    int calculateTotalApartments() const {
        int total = 0;
        for (int i = 0; i < size; ++i) {
            total += buildings[i].apartmentsCount;
        }
        return total;
    }

    // ������ ������ �� ���������� �����
    void readFromTextFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "������ �������� �����: " << filename << "\n";
            return;
        }
        size = 0; // ����� �������� ������� �������
        string street;
        int houseNumber, apartmentsCount;
        while (file >> street >> houseNumber >> apartmentsCount) {
            addBuilding(street, houseNumber, apartmentsCount);
        }
        file.close();
    }

    // ������ ������ �� ��������� �����
    void readFromBinaryFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "������ �������� �����: " << filename << "\n";
            return;
        }
        size = 0; // ����� �������� ������� �������
        Building temp;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Building))) {
            addBuilding(temp.street, temp.houseNumber, temp.apartmentsCount);
        }
        file.close();
    }

    // ������ ������ � ��������� ����
    void writeToTextFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "������ �������� �����: " << filename << "\n";
            return;
        }
        for (int i = 0; i < size; ++i) {
            file << buildings[i].street << " " << buildings[i].houseNumber << " " << buildings[i].apartmentsCount << "\n";
        }
        file.close();
    }

    // ������ ������ � �������� ����
    void writeToBinaryFile(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "������ �������� �����: " << filename << "\n";
            return;
        }
        for (int i = 0; i < size; ++i) {
            file.write(reinterpret_cast<const char*>(&buildings[i]), sizeof(Building));
        }
        file.close();
    }

    // ������ ������� �����
    long getFileSize(const string& filename) const {
        ifstream file(filename, ios::binary | ios::ate);
        if (!file) {
            cerr << "������ �������� �����: " << filename << "\n";
            return -1;
        }
        return file.tellg();
    }
};

// ������� ��� ����������� ����
void printMenu() {
    cout << "\n����:\n";
    cout << "1. ���� ������ � ����������\n";
    cout << "2. ������ ������ �� ���������� �����\n";
    cout << "3. ������ ������ �� ��������� �����\n";
    cout << "4. ����� ������ �� �����\n";
    cout << "5. ������ ������ ���������� �������\n";
    cout << "6. ���������� ������\n";
    cout << "7. �������� ������\n";
    cout << "8. ������ ������ � ��������� ����\n";
    cout << "9. ������ ������ � �������� ����\n";
    cout << "10. ������ ������� �����\n";
    cout << "11. �����\n";
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
        cout << "�������� ��������: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "������� �����: ";
            cin >> street;
            cout << "������� ����� ����: ";
            cin >> houseNumber;
            cout << "������� ���������� �������: ";
            cin >> apartmentsCount;
            db.addBuilding(street, houseNumber, apartmentsCount);
            break;
        }
        case 2: {

            cout << "������� ��� ���������� �����: ";
            cin >> filename;
            db.readFromTextFile(filename);
            break;
        }
        case 3:
        {
            cout << "������� ��� ��������� �����: ";
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
            cout << "����� ���������� �������: " << db.calculateTotalApartments() << "\n";
            break;
        }
        case 6:
        {
            cout << "������� �����: ";
            cin >> street;
            cout << "������� ����� ����: ";
            cin >> houseNumber;
            cout << "������� ���������� �������: ";
            cin >> apartmentsCount;
            db.addBuilding(street, houseNumber, apartmentsCount);
            break;
        }
        case 7:
        {
            cout << "������� ������ ������ ��� ��������: ";
            cin >> houseNumber;
            db.deleteBuilding(houseNumber - 1); // ���������� � 0
            break;
        }
        case 8:
        {
            cout << "������� ��� ���������� �����: ";
            cin >> filename;
            db.writeToTextFile(filename);
            break;
        }
        case 9:
        {
            cout << "������� ��� ��������� �����: ";
            cin >> filename;
            db.writeToBinaryFile(filename);
            break;
        }
        case 10:
        {
            cout << "������� ��� �����: ";
            cin >> filename;
            long size = db.getFileSize(filename);
            if (size != -1) {
                cout << "������ �����: " << size << " ����\n";
            }

            break;
        }
        case 11:
        {
            cout << "����� �� ���������.\n";
            break;
        }
        default:
        {
            cout << "�������� �����. ���������� �����.\n";
        }
        }
    } while (choice != 11);

    return 0;
}