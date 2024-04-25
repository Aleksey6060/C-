#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

using namespace std;


class Algae {
public:
    Algae(const string& type, int quantity) : type(type), quantity(quantity) {}
    virtual ~Algae() = default;

    virtual void grow() { quantity++; }
    virtual string getType() const { return type; }
    virtual int getQuantity() const { return quantity; }

protected:
    string type;
    int quantity;
};



class Fish {
public:
    Fish(const string& name, const string& type, int size) : name(name), type(type), size(size), feedCount(0) {}
    virtual ~Fish() = default;

    virtual void move() { cout << "Рыба " << name << " перемещается." << endl; }
    virtual bool isStarving() const { return false; };
    virtual void kill() { cout << "Рыбка " << name << " была убита." << endl; }
    virtual string getName() const { return name; }
    virtual string getType() const { return type; }
    virtual int getSize() const { return size; }

    void feed() {
        feedCount++;
        if (feedCount >= 2) {
            size++;
            cout << "Рыбка " << name << " была покормлена и её размер увеличен на 1." << endl;
            feedCount = 0;
        }
        else {
            cout << "Рыбка " << name << " была покормлена." << endl;
        }
    }

protected:
    string name;
    string type;
    int size;
    int feedCount;
};


class Aquarium {
public:
    void addFish(shared_ptr<Fish> fish) { fishList.push_back(fish); }
    void addAlgae(shared_ptr<Algae> algae) { algaeList.push_back(algae); }

    void update() {
        
        for (auto& fish : fishList) {
            fish->move();
            if (fish->isStarving()) {
                fish->kill();
            }
        }
        
        for (auto& algae : algaeList) {
            algae->grow();
        }
    }

    void display() {
        
        for (auto& fish : fishList) {
            cout << "Рыба: " << fish->getName() << ", Тип: " << fish->getType() << ", Размер: " << fish->getSize() << endl;
        }
        
        for (auto& algae : algaeList) {
            cout << "Водоросль: " << algae->getType() << ", Количество: " << algae->getQuantity() << endl;
        }
    }

    void feedFish() {
        if (fishList.empty()) {
            cout << "В аквариуме нет рыб для кормления." << endl;
            return;
        }

        cout << "Выберите рыбку для кормления:" << endl;
        for (size_t i = 0; i < fishList.size(); ++i) {
            cout << i + 1 << ". " << fishList[i]->getName() << endl;
        }

        int choice;
        cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(fishList.size())) {
            fishList[choice - 1]->feed();
        }
        else {
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
        }
    }

    void addNewFish() {
        string name, type;
        int size;
        cout << "Введите имя новой рыбки: ";
        cin >> name;
        cout << "Введите тип новой рыбки: ";
        cin >> type;
        cout << "Введите размер новой рыбки: ";
        cin >> size;
        addFish(make_shared<Fish>(name, type, size));
        cout << "Новая рыбка добавлена в аквариум." << endl;
    }

    void killFish() {
        if (fishList.empty()) {
            cout << "В аквариуме нет рыб для убийства." << endl;
            return;
        }

        cout << "Выберите рыбку для убийства:" << endl;
        for (size_t i = 0; i < fishList.size(); ++i) {
            cout << i + 1 << ". " << fishList[i]->getName() << endl;
        }

        int choice;
        cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(fishList.size())) {
            fishList[choice - 1]->kill();
            fishList.erase(fishList.begin() + (choice - 1));
        }
        else {
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
        }
    }

    void growAlgae() {
        for (auto& algae : algaeList) {
            algae->grow();
        }
    }

private:
    vector<shared_ptr<Fish>> fishList;
    vector<shared_ptr<Algae>> algaeList;
};

int main() {
    setlocale(LC_ALL, "Russian");
    Aquarium aquarium;

    
    aquarium.addFish(make_shared<Fish>("Травоядное животное", "Травоядное животное", 5));
    aquarium.addFish(make_shared<Fish>("Хищное животное", "Хищное животное", 8));
    aquarium.addAlgae(make_shared<Algae>("Зеленая водоросль", 10));

    while (true) {
        cout << "\nГлавное меню:" << endl;
        cout << "1. Обновить аквариум." << endl;
        cout << "2. Показать текущее состояние аквариума." << endl;
        cout << "3. Добавить новую рыбку." << endl;
        cout << "4. Кормить рыбку." << endl;
        cout << "5. Вырастить водоросли." << endl;
        cout << "6. Убить рыбку." << endl;
        cout << "7. Выйти." << endl;
        cout << "Введите ваш выбор: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            aquarium.update();
            break;
        case 2:
            aquarium.display();
            break;
        case 3:
            aquarium.addNewFish();
            break;
        case 4:
            aquarium.feedFish();
            break;
        case 5:
            aquarium.growAlgae();
            break;
        case 6:
            aquarium.killFish();
            break;
        case 7:
            std::cout << "Выход..." << std::endl;
            return 0;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    }

    return 0;
}