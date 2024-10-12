#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Item {
    std::string name;
    float price;
    int quantity;

    Item(std::string n, float p, int q) : name(n), price(p), quantity(q) {}
};

class Merchant {
private:
    std::vector<Item> inventory;

public:
    Merchant() {
        inventory.push_back(Item("Throwing Knife", 15.0f, 22));
        inventory.push_back(Item("Health Potion", 5.0f, 10));
        inventory.push_back(Item("Shield", 16.0f, 1));
        inventory.push_back(Item("Mage's Lost Wand", 23.0f, 1));
        inventory.push_back(Item("Cooked Chicken", 8.0f, 7));
        inventory.push_back(Item("Ring of Protection", 21.0f, 1));
        inventory.push_back(Item("Mana Potion", 6.0f, 5));
    }

    void displayInventory() const {
        std::cout << "Merchant Inventory:\n";
        for (const auto& item : inventory) {
            std::cout << item.name << " - Price: $" << item.price
                << ", Quantity: " << (item.quantity > 0 ? std::to_string(item.quantity) : "SOLD OUT") << "\n";
        }
        std::cout << "\n";
    }

    bool sellItem(const std::string& itemName, int quantity, float& playerMoney) {
        for (auto& item : inventory) {
            if (item.name == itemName) {
                if (item.quantity == 0) {
                    std::cout << item.name << " is SOLD OUT.\n\n";
                    return false;
                }
                if (item.quantity < quantity) {
                    std::cout << "Not enough " << item.name << " in stock.\n\n";
                    return false;
                }
                float totalCost = item.price * quantity;
                if (playerMoney < totalCost) {
                    std::cout << "Not enough money to buy " << quantity << " of " << item.name << ".\n\n";
                    return false;
                }
                item.quantity -= quantity;
                playerMoney -= totalCost;
                std::cout << "Sold " << quantity << " of " << item.name << ".\n\n";
                return true;
            }
        }
        std::cout << itemName << " not found.\n\n";
        return false;
    }

    float getItemPrice(const std::string& itemName) const {
        for (const auto& item : inventory) {
            if (item.name == itemName) {
                return item.price;
            }
        }
        return 0.0f;
    }

    void buyItem(const std::string& itemName, int quantity, float& playerMoney, std::vector<Item>& playerInventory) {
        for (auto& item : playerInventory) {
            if (item.name == itemName) {
                if (item.quantity < quantity) {
                    std::cout << "Not enough " << item.name << " in your inventory.\n\n";
                    return;
                }
                float sellPrice = item.price * 0.5f;
                playerMoney += sellPrice * quantity;
                item.quantity -= quantity;

                if (item.quantity == 0) {
                    playerInventory.erase(std::remove_if(playerInventory.begin(), playerInventory.end(),
                        [&item](const Item& i) { return i.name == item.name; }),
                        playerInventory.end());
                }

                for (auto& merchantItem : inventory) {
                    if (merchantItem.name == itemName) {
                        merchantItem.quantity += quantity;
                        std::cout << "Bought " << quantity << " of " << item.name << " from you.\n\n";
                        return;
                    }
                }
                inventory.push_back(Item(item.name, sellPrice, quantity));
                std::cout << "Bought " << quantity << " of " << item.name << " from you.\n\n";
                return;
            }
        }
        std::cout << itemName << " not found in your inventory.\n\n";
    }
};

class Player {
public:
    float money;
    std::vector<Item> inventory;

    Player() : money(100.0f) {}

    void addItem(const std::string& name, float price, int quantity) {
        for (auto& item : inventory) {
            if (item.name == name) {
                item.quantity += quantity;
                return;
            }
        }
        inventory.push_back(Item(name, price, quantity));
    }

    void displayInventory() const {
        std::cout << "Player Inventory:\n";
        std::cout << "Money: $" << money << "\n";
        for (const auto& item : inventory) {
            std::cout << item.name << " - Quantity: " << item.quantity << "\n";
        }
        std::cout << "\n";
    }
};

int main() {
    std::string playerName;
    Player player;
    Merchant merchant;

    std::cout << "Enter your name: ";
    std::getline(std::cin, playerName);

    std::cout << "\nWelcome " << playerName << ", Hero of Gravestone! You currently have $" << player.money << " in your account.\n\n";
    merchant.displayInventory();

    char choice;

    while (true) {
        std::cout << "Press 'B' to buy, 'S' to sell, or 'I' to check your inventory (or 'exit' to quit): ";
        std::cin >> choice;
        std::cin.ignore();  // Ignore the newline character

        if (choice == 'e' || choice == 'E') {
            break;
        }

        switch (choice) {
        case 'B':
        case 'b': {
            std::string itemName;
            int quantity;

            std::cout << "Enter the name of the item you want to buy: ";
            std::getline(std::cin, itemName);

            std::cout << "Enter quantity: ";
            std::cin >> quantity;
            std::cin.ignore();

            if (merchant.sellItem(itemName, quantity, player.money)) {
                player.addItem(itemName, merchant.getItemPrice(itemName), quantity);
                std::cout << "Updated Merchant Inventory:\n";
                merchant.displayInventory();
            }

            std::cout << "Player Money: $" << player.money << "\n\n";
            break;
        }

        case 'S':
        case 's': {
            std::string itemName;
            int quantity;

            std::cout << "Enter the name of the item you want to sell: ";
            std::getline(std::cin, itemName);
            std::cout << "Enter quantity: ";
            std::cin >> quantity;
            std::cin.ignore();

            merchant.buyItem(itemName, quantity, player.money, player.inventory);
            player.displayInventory();
            break;
        }

        case 'I':
        case 'i': {
            player.displayInventory();
            break;
        }

        default:
            std::cout << "Invalid choice. Please try again.\n\n";
        }
    }

    return 0;
}
