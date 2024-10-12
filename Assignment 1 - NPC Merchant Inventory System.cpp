//Assignment 1 - NPC Merchant Inventory System
//Liana Hockin
//October 11th 2024

//include all the libraries! 
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// structure to represent an item
struct Item {
    std::string name; // name of the item
    float price; // price of the item
    int quantity; // quantity available of the item
    std::string buffDescription; // description of the buffs

    //constructor to initialize an Item
    Item(std::string n, float p, int q, std::string buff = "")
        : name(n), price(p), quantity(q), buffDescription(buff) {}
};

//Initiate the merchant class
class Merchant {
private: //private is only for this class
    std::vector<Item> inventory;
    std::vector<Item> animalInventory;

public: // public is to be able to be used for all classes! 
    Merchant() {
        inventory.push_back(Item("Throwing Knife", 15.0f, 22)); //push_back is better than emplace_back sir says. Push uses the copy. 
        inventory.push_back(Item("Health Potion", 5.0f, 10));
        inventory.push_back(Item("Shield", 16.0f, 1));
        inventory.push_back(Item("Mage's Lost Wand", 23.0f, 1));
        inventory.push_back(Item("Cooked Chicken", 8.0f, 7));
        inventory.push_back(Item("Ring of Protection", 21.0f, 1));
        inventory.push_back(Item("Mana Potion", 6.0f, 5));

        animalInventory.push_back(Item("Riley the Black Cat", 100.0f, 1, "Buff: Stealth")); // wanted to make a seperate inventory for animals still all within the merchant class
        animalInventory.push_back(Item("Pumpkin the Orange Cat", 100.0f, 1, "Buff: Luck")); // these buffs do nothing, theyre just for show
        animalInventory.push_back(Item("Conor the Black Lab", 100.0f, 1, "Buff: Strength"));
    }

    // display the merchant inventory and animals
    void displayInventory() const { //const = constant! it is unchangeable! 
        std::cout << "Merchant Inventory:\n";
        for (const auto& item : inventory) { //auto makes it easier to cycle through the items !!!
            std::cout << item.name << " - Price: $" << item.price
                << ", Quantity: " << (item.quantity > 0 ? std::to_string(item.quantity) : "SOLD OUT") << "\n";
        }
        std::cout << "\nAnimal Inventory:\n";
        for (const auto& animal : animalInventory) {
            std::cout << animal.name << " - Price: $" << animal.price
                << ", Quantity: " << (animal.quantity > 0 ? std::to_string(animal.quantity) : "SOLD OUT")
                << " (" << animal.buffDescription << ")\n";
        }
        std::cout << "\n";
    }

    // function to sell the items
    bool sellItem(const std::string& itemName, int quantity, float& playerMoney) {
        for (auto& item : inventory) {
            if (item.name == itemName) {
                if (item.quantity == 0) {
                    std::cout << item.name << " is SOLD OUT.\n\n";
                    return false; // item is sold out
                }
                if (item.quantity < quantity) {
                    std::cout << "Not enough " << item.name << " in stock.\n\n";
                    return false;  // Not enough quantity
                }
                float totalCost = item.price * quantity;
                if (playerMoney < totalCost) {
                    std::cout << "Not enough money to buy " << quantity << " of " << item.name << ".\n\n";
                    return false; // Not enough money
                }
                item.quantity -= quantity;
                playerMoney -= totalCost;
                std::cout << "Sold " << quantity << " of " << item.name << ".\n\n";
                return true; // return true when able to sell the item
            }
        }

        // check to see if item is in the animal inventory now! 
        for (auto& animal : animalInventory) {
            if (animal.name == itemName) {
                if (animal.quantity == 0) {
                    std::cout << animal.name << " is SOLD OUT.\n\n";
                    return false; // Animal is sold out
                }
                if (animal.quantity < quantity) {
                    std::cout << "Not enough " << animal.name << " in stock.\n\n";
                    return false; // Not enough quantity
                }
                float totalCost = animal.price * quantity;
                if (playerMoney < totalCost) {
                    std::cout << "Not enough money to buy " << quantity << " of " << animal.name << ".\n\n";
                    return false; // Not enough money
                }
                animal.quantity -= quantity;
                playerMoney -= totalCost;
                std::cout << "Sold " << quantity << " of " << animal.name << ".\n\n";
                return true; // return true when able to sell the item
            }
        }
        std::cout << itemName << " not found.\n\n";
        return false; // Sale failed for some other reason 
    }

    // Function to get the price of an item by name
    float getItemPrice(const std::string& itemName) const {
        for (const auto& item : inventory) {
            if (item.name == itemName) {
                return item.price; // return the price if found for items
            }
        }
        for (const auto& animal : animalInventory) {
            if (animal.name == itemName) {
                return animal.price; // return the price if found for animals
            }
        }
        return 0.0f; // return no money if not found
    }

    //Function to buy an item from the player !
    void buyItem(const std::string& itemName, int quantity, float& playerMoney, std::vector<Item>& playerInventory) {
        //check to see if the item is in the players inventory first
        for (auto& item : playerInventory) {
            if (item.name == itemName) {
                if (item.quantity < quantity) {
                    std::cout << "Not enough " << item.name << " in your inventory.\n\n";
                    return; //doesnt have enough in the players inventory
                }
                float sellPrice = item.price * 0.5f; // its basically saying x by 0.5 which is 50 percent lower when sold back to the merchant, wanted to add this to make it seem more realistic! 
                playerMoney += sellPrice * quantity; // add the money to the players account
                item.quantity -= quantity; // deduct the quantity 
                if (item.quantity == 0) { // remove from players inventory if quanity reaches 0! 
                    playerInventory.erase(std::remove_if(playerInventory.begin(), playerInventory.end(),
                        [&item](const Item& i) { return i.name == item.name; }),
                        playerInventory.end());
                }
                for (auto& merchantItem : inventory) { // does this item already exsist in the merchants inventory?
                    if (merchantItem.name == itemName) { // == means if its the same thing! 
                        merchantItem.quantity += quantity; // increase the merchants quantity 
                        std::cout << "Bought " << quantity << " of " << item.name << " from you.\n\n";
                        return; // if the buying is successful and the player has enough to purchase
                    }
                }
                inventory.push_back(Item(item.name, sellPrice, quantity)); // if not found, add to the merchants inventory by using pushback which copies it
                std::cout << "Bought " << quantity << " of " << item.name << " from you.\n\n";
                return; // if the buying is successful 
            }
        }
        std::cout << itemName << " not found in your inventory.\n\n"; // the item was not found!
    }

    // feature i added extra, to able the player to talk to the merchant! creates a more deeper story within the NPC Inventory
    void talkToMikage(const std::string& playerName) const {
        std::cout << "Mikage: Greetings, " << playerName << ". I am Mikage, the merchant of these lands. What would you like to know?\n"
            << "\n1. Tell me about your items.\n"
            << "2. What's your story?\n"
            << "3. Just browsing.\n";

        int choice;
        std::cin >> choice; // cin for inserting the players choice

        //the players choice between 1, 2 and 3 and then the default answer
        switch (choice) {
        case 1:
            std::cout << "\nMikage: I have many items from health potions to powerful weapons for you to use on your adventure " << playerName << "! \n\n";
            break;
        case 2: // wanted to really add a spin on things and make the player interested in the life of the merchant!! 
            std::cout << "\nMikage: Ah, As a young prince, I once stood against my parents' oppressive rule, unable to bear their tyranny.\n"
                "I escaped the royal life, determined to fight against their evil, but for reasons I still do not fully understand, I lost my powers.\n"
                "So, I have lived as a humble merchant, quietly plotting to thwart their reign.\n"
                "As a way to get back at me, my such loving parents kidnapped my best friend, holding him hostage within my very own place of my birth.\n"
                "Over the past few months, I have been uncovering the truth behind my lost strength, and I can feel it awakening within me, one day at a time.\n"
                "I know that one day soon, I will reclaim my rightful place on the throne and restore justice to my kingdom.\n\n";
            break;
        case 3:
            std::cout << "\nMikage: Very well! Let me know if you need anything, I am always here for you! \n\n";
            break;
        default:
            std::cout << "\nMikage: I don't understand that, what do you mean young hero?\n\n"; // anything else brings them back to this default line seen alot in other games hehe i got inspiration!
        }
    }
};

// class for the player!
class Player {
public:
    float money; // players money! i use float, since floats can add decimals used for money
    std::vector<Item> inventory; // players inventory of items! 

    Player() : money(100.0f) {} // initializing the players money at $100

    // function to add an item to the players inventory
    void addItem(const std::string& name, float price, int quantity) {
        for (auto& item : inventory) {
            if (item.name == name) {
                item.quantity += quantity; // if item exists, then increase the quantity
                return;
            }
        }
        inventory.push_back(Item(name, price, quantity)); // add new item if it doesnt already exist
    }

    //display the players inventory
    void displayInventory() const {
        std::cout << "\nPlayer Inventory:\n";
        std::cout << "Money: $" << money << "\n"; // show the money they own
        for (const auto& item : inventory) {
            std::cout << item.name << " - Quantity: " << item.quantity << "\n"; // show the item and the amount they own
        }
        std::cout << "\n"; // this just makes another line, easier to read on my end! \n is a lifesaver
    }
}; // make sure to add semicolons when closing

//now that all the classes are written we can get into actually building the code itself

// Main function to run the game! very important! in my eyes, this is like LUA LOVE2D functions all put together to run what it will do and look like
int main() {
    std::string playerName;  //variable holding the players inputted name
    Player player; // create a player object and merchant object
    Merchant merchant;

    // get the players name by asking them and then getting the cin they type
    std::cout << "Enter your name: ";
    std::getline(std::cin, playerName);

    //welcome message to the player once they type their name in! 
    std::cout << "\nWelcome " << playerName << ", Hero of Gravestone! You currently have $" << player.money << " in your account.\n"
        << "You can sell items to me at 50% of the original selling price. Sorry, a guys gotta make a living, " << playerName << ".\n\n";

    char choice; // varaible to store all the players choices! i learned that char is used to store one character at a time. so in this case its one choice at a time so the program lets the player press B or S or I or T or E! 

    //main game loop required in the assignment
    while (true) {
        std::cout << "Press 'B' to buy, 'S' to sell, 'I' to check your inventory, 'T' to talk to Mikage, or 'E' to exit: "; // this cout shows the player the options 
        std::cin >> choice; //cin takes the players choice that they input
        std::cin.ignore();

        if (choice == 'e' || choice == 'E') { // i used both uppercase and lowercase to make it easier for the players
            break; // exit the loop if the player chooses the letter E
        }

        switch (choice) { // learned online that switches can be cleaner than else-if statments when it comes to clearly outlining different cases based on a players input (use this in the future if sir says its alright)
        case 'B':
        case 'b': {
            merchant.displayInventory(); // display the merchants inventory
            std::string itemName; //need to make sure its a string to be able to hold text since the player will be writing down what they want to buy!
            int quantity;
            std::cout << "\nEnter the name of the item you want to buy: ";
            std::getline(std::cin, itemName); // input the items name that the player wants to buy
            std::cout << "Enter quantity: ";
            std::cin >> quantity; // input the quantity
            std::cin.ignore();

            if (merchant.sellItem(itemName, quantity, player.money)) {
                player.addItem(itemName, merchant.getItemPrice(itemName), quantity); // add the item to the players inventory if the sale is successful! 
                std::cout << "Player Money: $" << player.money << "\n\n"; //show the updated player money amount after the sale
            }
            break; // exit after the transaction
        }

        case 'S': // basically the same thing as buying. the user inputs the item name and quantity they want
        case 's': {
            std::string itemName;
            int quantity;
            std::cout << "\nEnter the name of the item you want to sell: ";
            std::getline(std::cin, itemName);
            std::cout << "Enter quantity: ";
            std::cin >> quantity;
            std::cin.ignore();
            merchant.buyItem(itemName, quantity, player.money, player.inventory); // this lets the player sell the specific item to the merchant
            player.displayInventory(); // display the updated players inventory
            break;
        }

        case 'I':
        case 'i': {
            player.displayInventory(); // displays the players inventory by pressing i 
            break;
        }

        case 'T':
        case 't': {
            merchant.talkToMikage(playerName);  // lets the player talk to the merchant by pressing t 
            break;
        }

        default:
            std::cout << "\nInvalid choice. Please try again.\n\n"; // any other option brings the player here. this is like an else statement from what i learned
        }
    }

    return 0; // always need to include this to end the program!!! important liana!!! 
}