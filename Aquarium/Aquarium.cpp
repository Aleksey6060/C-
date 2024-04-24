#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <Windows.h>

class Fish {
public:
    Fish(std::string name, std::string type, int size)
        : name(name), type(type), size(size), hungerLevel(0), isAlive(true) {}

    std::string getName() const {
        return name;
    }

    std::string getType() const {
        return type;
    }

    int getSize() const {
        return size;
    }

    int getHungerLevel() const {
        return hungerLevel;
    }

    bool isStarving() const {
        return hungerLevel >= maxHungerLevel;
    }

    bool isAliveStatus() const {
        return isAlive;
    }

    void move() {
        hungerLevel++;
    }

    void eat() {
        hungerLevel = 0; // После еды уровень голода сбрасывается
    }

    virtual bool reproduce() {
        // Условия размножения (например, уровень голода должен быть низким и т.п.)
        return hungerLevel <= maxHungerLevel / 2;
    }

    void kill() {
        isAlive = false;
    }

private:
    std::string name;
    std::string type;
    int size;
    int hungerLevel;
    bool isAlive;
    const int maxHungerLevel = 5; // Установлено значение максимального голода
};

class Algae {
public:
    Algae(std::string type, int quantity) : type(type), quantity(quantity) {}

    std::string getType() const {
        return type;
    }

    int getQuantity() const {
        return quantity;
    }

    void grow() {
        quantity += growthRate; // Рост водорослей
    }

    void consume(int amount) {
        if (quantity >= amount) {
            quantity -= amount; // Уменьшение количества водорослей при потреблении
        }
    }

private:
    std::string type;
    int quantity;
    const int growthRate = 5; // Скорость роста водорослей
};

class Aquarium {
public:
    void addFish(std::shared_ptr<Fish> fish) {
        fishList.push_back(fish);
    }

    void addAlgae(std::shared_ptr<Algae> algae) {
        algaeList.push_back(algae);
    }

    void update() {
        // Обновление состояния аквариума
        for (size_t i = 0; i < fishList.size(); i++) {
            auto fish = fishList[i];
            // Движение рыб
            fish->move();

            // Проверка голода
            if (fish->isStarving()) {
                std::cout << fish->getName() << " умерла от голода." << std::endl;
                fish->kill();
            }

            // Взаимодействие между рыбой и водорослями
            if (fish->getType() == "Herbivorous Fish" && fish->isAliveStatus()) {
                auto foundAlgae = false;
                for (auto& algae : algaeList) {
                    if (algae->getQuantity() > 0) {
                        foundAlgae = true;
                        fish->eat();
                        algae->consume(1);
                        break;
                    }
                }
                if (!foundAlgae) {
                    std::cout << fish->getName() << " умерла от голода, так как нет водорослей." << std::endl;
                    fish->kill();
                }
            }

            // Взаимодействие между хищными и простыми рыбами
            if (fish->getType() == "Carnivorous Fish" && fish->isAliveStatus()) {
                for (auto& otherFish : fishList) {
                    if (otherFish->getType() != "Carnivorous Fish" && fish->getSize() > otherFish->getSize() && otherFish->isAliveStatus()) {
                        fish->eat();
                        otherFish->kill();
                        std::cout << fish->getName() << " съела " << otherFish->getName() << "." << std::endl;
                        break;
                    }
                }
            }
        }

        // Удаление мертвых рыб
        fishList.erase(std::remove_if(fishList.begin(), fishList.end(), [](std::shared_ptr<Fish> fish) {
            return !fish->isAliveStatus();
            }), fishList.end());

        // Размножение рыб
        for (auto& fish : fishList) {
            if (fish->reproduce()) {
                auto newFish = std::make_shared<Fish>(fish->getName(), fish->getType(), fish->getSize());
                addFish(newFish);
                std::cout << fish->getName() << " размножилась." << std::endl;
            }
        }

        // Рост водорослей
        for (auto& algae : algaeList) {
            algae->grow();
        }

        showUpdateMenu();
    }

    void display() {
        std::cout << "Текущее состояние аквариума:" << std::endl;
        std::cout << "Рыбы:" << std::endl;
        for (const auto& fish : fishList) {
            std::cout << "- " << fish->getName() << " (" << fish->getType() << ", размер: " << fish->getSize() << ", уровень голода: " << fish->getHungerLevel() << ")" << std::endl;
        }
        std::cout << "Водоросли:" << std::endl;
        for (const auto& algae : algaeList) {
            std::cout << "- " << algae->getType() << " (количество: " << algae->getQuantity() << ")" << std::endl;
        }
    }

    void showUpdateMenu() {
        std::cout << "\nМеню обновления:" << std::endl;
        std::cout << "1. Убить рыбку." << std::endl;
        std::cout << "2. Покормить рыбку." << std::endl;
        std::cout << "3. Размножить рыбку." << std::endl;
        std::cout << "4. Вырастить водоросли." << std::endl;
        std::cout << "Введите ваш выбор: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Очистить буфер после ввода

        switch (choice) {
        case 1:
            killFish();
            break;
        case 2:
            feedFish();
            break;
        case 3:
            reproduceFish();
            break;
        case 4:
            growAlgae();
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            showUpdateMenu(); // Повторно показать меню в случае неверного выбора
            break;
        }
    }

    void killFish() {
        std::cout << "Введите имя рыбки для убийства: ";
        std::string name;
        std::getline(std::cin, name);

        bool foundFish = false;
        for (auto& fish : fishList) {
            if (fish->getName() == name && fish->isAliveStatus()) {
                fish->kill();
                std::cout << name << " убита." << std::endl;
                foundFish = true;
                break;
            }
        }
        if (!foundFish) {
            std::cout << "Рыбка " << name << " не найдена." << std::endl;
        }
    }

    void feedFish() {
        std::cout << "Введите имя рыбки для кормления: ";
        std::string name;
        std::getline(std::cin, name);

        bool foundFish = false;
        for (auto& fish : fishList) {
            if (fish->getName() == name && fish->isAliveStatus()) {
                fish->eat();
                std::cout << name << " покормлена." << std::endl;
                foundFish = true;
                break;
            }
        }
        if (!foundFish) {
            std::cout << "Рыбка " << name << " не найдена." << std::endl;
        }
    }

    void reproduceFish() {
        std::cout << "Введите имя рыбки для размножения: ";
        std::string name;
        std::getline(std::cin, name);

        bool foundFish = false;
        for (auto& fish : fishList) {
            if (fish->getName() == name && fish->isAliveStatus() && fish->reproduce()) {
                auto newFish = std::make_shared<Fish>(name, fish->getType(), fish->getSize());
                addFish(newFish);
                std::cout << name << " размножилась." << std::endl;
                foundFish = true;
                break;
            }
        }
        if (!foundFish) {
            std::cout << "Рыбка " << name << " не найдена или не может размножиться." << std::endl;
        }
    }

    void growAlgae() {
        for (auto& algae : algaeList) {
            algae->grow();
            std::cout << algae->getType() << " выросли." << std::endl;
        }
    }

    void addNewFish() {
        std::cout << "Введите имя рыбки: ";
        std::string name;
        std::getline(std::cin, name);

        std::cout << "Введите тип рыбки: ";
        std::string type;
        std::getline(std::cin, type);

        std::cout << "Введите размер рыбки: ";
        int size;
        std::cin >> size;
        std::cin.ignore(); // Очистить буфер после ввода

        auto newFish = std::make_shared<Fish>(name, type, size);
        addFish(newFish);
        std::cout << "Рыбка " << name << " добавлена в аквариум." << std::endl;
    }

private:
    std::vector<std::shared_ptr<Fish>> fishList;
    std::vector<std::shared_ptr<Algae>> algaeList;
};

int main() {
    setlocale(LC_ALL, "Russian");
    Aquarium aquarium;

    // Добавим начальных рыб и водоросли в аквариум
    aquarium.addFish(std::make_shared<Fish>("Herbivorous Fish", "Herbivorous Fish", 5));
    aquarium.addFish(std::make_shared<Fish>("Carnivorous Fish", "Carnivorous Fish", 8));
    aquarium.addAlgae(std::make_shared<Algae>("Green Algae", 10));

    int choice = 0;
    while (choice != 4) {
        std::cout << "\nГлавное меню:" << std::endl;
        std::cout << "1. Обновить аквариум." << std::endl;
        std::cout << "2. Показать текущее состояние аквариума." << std::endl;
        std::cout << "3. Добавить новую рыбку." << std::endl;
        std::cout << "4. Выйти." << std::endl;
        std::cout << "Введите ваш выбор: ";

        std::cin >> choice;
        std::cin.ignore(); // Очистить буфер после ввода

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
            std::cout << "Выход..." << std::endl;
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз." << std::endl;
            break;
        }
    }

    return 0;
}
