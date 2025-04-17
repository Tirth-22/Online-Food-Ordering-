#include<bits/stdc++.h>
#include <conio.h>
using namespace std;

// Base MenuItem class
class MenuItem {
protected:
    string name;
    float price;
public:
    MenuItem(const string& n, float p) : name(n), price(p) {}
    virtual ~MenuItem() = default;
    
    virtual void display() const {
        cout << name << " - Rs." << fixed << setprecision(2) << price;
    }
    
    virtual unique_ptr<MenuItem> clone() const = 0;
    
    float getPrice() const { return price; }
    string getName() const { return name; }
    
    bool operator<(const MenuItem& other) const {
        return price < other.price;
    }
    
    friend ostream& operator<<(ostream& os, const MenuItem& item);
};

ostream& operator<<(ostream& os, const MenuItem& item) {
    item.display();
    return os;
}

// Derived classes
class Pizza : public MenuItem {
    string size;
public:
    Pizza(const string& n, float p, const string& s) : MenuItem(n, p), size(s) {}
    
    void display() const override {
        cout << size << " " << name << " - Rs." << fixed << setprecision(2) << price;
    }
    
    unique_ptr<MenuItem> clone() const override {
        return make_unique<Pizza>(*this);
    }
};

class Burger : public MenuItem {
public:
    Burger(const string& n, float p) : MenuItem(n, p) {}
    
    unique_ptr<MenuItem> clone() const override {
        return make_unique<Burger>(*this);
    }
};

class Sandwich : public MenuItem {
public:
    Sandwich(const string& n, float p) : MenuItem(n, p) {}
    
    unique_ptr<MenuItem> clone() const override {
        return make_unique<Sandwich>(*this);
    }
};

class Roll : public MenuItem {
public:
    Roll(const string& n, float p) : MenuItem(n, p) {}
    
    unique_ptr<MenuItem> clone() const override {
        return make_unique<Roll>(*this);
    }
};

class Biryani : public MenuItem {
public:
    Biryani(const string& n, float p) : MenuItem(n, p) {}
    
    unique_ptr<MenuItem> clone() const override {
        return make_unique<Biryani>(*this);
    }
};

// Order class
class Order {
    vector<unique_ptr<MenuItem>> items;
    map<string, int> quantities;
    string customerName;
    
public:
    Order(const string& name) : customerName(name) {}
    
    void addItem(unique_ptr<MenuItem> item, int quantity) {
        items.push_back(move(item));
        quantities[items.back()->getName()] = quantity;
    }
    
    float calculateTotal() const {
        float total = 0;
        for (const auto& item : items) {
            total += item->getPrice() * quantities.at(item->getName());
        }
        return total;
    }
    
    void displayOrder() const {
        cout << "\n\t\t--------Order Summary---------\n";
        cout << "Customer: " << customerName << "\n\n";
        for (const auto& item : items) {
            cout << quantities.at(item->getName()) << " x ";
            item->display();
            cout << endl;
        }
        cout << fixed << setprecision(2);
        cout << "\nTotal: Rs." << calculateTotal() << endl;
    }
    
    void processPayment() const {
        float total = calculateTotal();
        cout << fixed << setprecision(2);
        cout << "\nTotal Amount Due: Rs." << total << endl;
        
        float amountPaid;
        while (true) {
            cout << "Enter amount paid: Rs.";
            cin >> amountPaid;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a valid amount.\n";
                continue;
            }
            
            if (amountPaid < total) {
                cout << "Insufficient amount! Please pay at least Rs." << total << endl;
            } else {
                break;
            }
        }
        
        if (amountPaid > total) {
            float change = amountPaid - total;
            cout << "Change to return: Rs." << change << endl;
        }
        
        cout << "Payment successful! Thank you for your purchase.\n";
    }
    
    class EmptyOrderException : public exception {
    public:
        const char* what() const noexcept override {
            return "Cannot process empty order!";
        }
    };
    
    void validateOrder() const {
        if (items.empty()) {
            throw EmptyOrderException();
        }
    }
    
    void saveToFile(const string& filename) const {
        ofstream outFile(filename, ios::app);
        if (!outFile) {
            throw runtime_error("Could not open file for writing!");
        }
        
        outFile << "Order for: " << customerName << "\n";
        for (const auto& item : items) {
            outFile << quantities.at(item->getName()) << " x " << item->getName() 
                    << " @ Rs." << fixed << setprecision(2) << item->getPrice() << "\n";
        }
        outFile << "Total: Rs." << calculateTotal() << "\n";
        outFile << "---------------------------------\n\n";
        outFile.close();
    }
};

// Menu class
class Menu {
    map<int, unique_ptr<MenuItem>> pizzaMenu;
    map<int, unique_ptr<MenuItem>> burgerMenu;
    map<int, unique_ptr<MenuItem>> sandwichMenu;
    map<int, unique_ptr<MenuItem>> rollMenu;
    map<int, unique_ptr<MenuItem>> biryaniMenu;
    
public:
    Menu() {
        // Initialize pizza menu
        pizzaMenu[1] = make_unique<Pizza>("7-Cheesey", 250, "Small");
        pizzaMenu[2] = make_unique<Pizza>("Margerita", 500, "Regular");
        pizzaMenu[3] = make_unique<Pizza>("Peri Peri", 900, "Large");
        pizzaMenu[4] = make_unique<Pizza>("Creamy Max", 350, "Medium");
        
        // Initialize burger menu
        burgerMenu[1] = make_unique<Burger>("Zinger Burger", 180);
        burgerMenu[2] = make_unique<Burger>("veg Burger", 150);
        burgerMenu[3] = make_unique<Burger>("cheese veg Burger", 160);
        
        // Initialize sandwich menu
        sandwichMenu[1] = make_unique<Sandwich>("Club Sandwich", 240);
        sandwichMenu[2] = make_unique<Sandwich>("Veg Crispy Sandwich", 160);
        sandwichMenu[3] = make_unique<Sandwich>("Extream Veg cheese Sandwich", 100);
        
        // Initialize roll menu
        rollMenu[1] = make_unique<Roll>("Peri Peri Roll", 150);
        rollMenu[2] = make_unique<Roll>("Cheese Mayo Roll", 100);
        rollMenu[3] = make_unique<Roll>("Veg Roll With Fries", 120);
        
        // Initialize biryani menu
        biryaniMenu[1] = make_unique<Biryani>("Veg Biryani", 160);
        biryaniMenu[2] = make_unique<Biryani>("Biryani with pulav", 220);
        biryaniMenu[3] = make_unique<Biryani>("Special Biryani", 140);
    }
    
    void displayCategory(int category) const {
        switch(category) {
            case 1: displayMenu("Pizzas", pizzaMenu); break;
            case 2: displayMenu("Burgers", burgerMenu); break;
            case 3: displayMenu("Sandwiches", sandwichMenu); break;
            case 4: displayMenu("Rolls", rollMenu); break;
            case 5: displayMenu("Biryani", biryaniMenu); break;
            default: throw invalid_argument("Invalid category!");
        }
    }
    
    template<typename T>
    void displayMenu(const string& title, const T& menu) const {
        cout << "\n\t--------" << title << "--------\n";
        for (const auto& item : menu) {
            cout << item.first << ") " << *item.second << endl;
        }
    }
    
    unique_ptr<MenuItem> getItem(int category, int choice) {
        switch(category) {
            case 1: return pizzaMenu.at(choice)->clone();
            case 2: return burgerMenu.at(choice)->clone();
            case 3: return sandwichMenu.at(choice)->clone();
            case 4: return rollMenu.at(choice)->clone();
            case 5: return biryaniMenu.at(choice)->clone();
            default: throw invalid_argument("Invalid category!");
        }
    }
};

// FastFoodApp class
class FastFoodApp {
    Menu menu;
    const string orderFilename = "orders.txt";
    
    void clearScreen() {
        system("cls");
    }
    
public:
    void run() {
        int mainChoice;
        do {
            clearScreen();
            cout << "\t\t\t------Tirth`s Fast Food-------\n\n";
            cout << "1) Place New Order\n";
            cout << "2) View Previous Orders\n";
            cout << "3) Exit\n";
            cout << "\nPlease Enter your Choice: ";
            cin >> mainChoice;
            cin.ignore();
            
            switch(mainChoice) {
                case 1: 
                    placeNewOrder();
                    break;
                case 2:
                    viewPreviousOrders();
                    cout << "\nPress any key to continue...";
                    getch();
                    break;
                case 3:
                    cout << "\nThank you for visiting Our Fast Food!\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    cout << "Press any key to continue...";
                    getch();
            }
        } while (mainChoice != 3);
    }
    
private:
    void placeNewOrder() {
        clearScreen();
        string name;
        cout << "\t\t\t------Tirth`s Fast Food-------\n\n";
        cout << "Please Enter Your Name: ";
        getline(cin, name);
        
        Order order(name);
        char continueOrder;
        
        do {
            clearScreen();
            displayMainMenu();
            
            try {
                processOrder(order);
                
                cout << "\nWould you like to order anything else? (Y/N): ";
                cin >> continueOrder;
                cin.ignore();
            } 
            catch (const exception& e) {
                cerr << "\nError: " << e.what() << endl;
                cout << "Press any key to continue...";
                getch();
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (toupper(continueOrder) == 'Y');
        
        try {
            clearScreen();
            order.validateOrder();
            order.displayOrder();
            order.processPayment();
            order.saveToFile(orderFilename);
            cout << "\nThank you for ordering from Our Fast Food Cafe!\n";
            cout << "Press any key to continue...";
            getch();
        } 
        catch (const exception& e) {
            cerr << "\nError: " << e.what() << endl;
            cout << "Press any key to continue...";
            getch();
        }
    }
    
    void viewPreviousOrders() {
        clearScreen();
        cout << "\t\t\t----------Previous Orders-----------\n\n";
        
        ifstream inFile(orderFilename);
        if (!inFile) {
            cout << "No previous orders found.\n";
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        
        inFile.close();
        cout << "\nPress any key to continue...";
        getch();
    }
    
    void displayMainMenu() const {
        cout << "\t\t\t\t--------Main Menu--------\n\n";
        cout << "1) Pizzas\n";
        cout << "2) Burgers\n";
        cout << "3) Sandwich\n";
        cout << "4) Rolls\n";
        cout << "5) Biryani\n\n";
    }
    
    void processOrder(Order& order) {
        int category, choice, quantity;
        
        cout << "\nPlease Enter your Choice: ";
        cin >> category;
        
        if (category < 1 || category > 5) {
            throw invalid_argument("Invalid menu category!");
        }
        
        menu.displayCategory(category);
        
        cout << "\nPlease Enter your item choice: ";
        cin >> choice;
        
        cout << "Please Enter Quantity: ";
        cin >> quantity;
        
        if (quantity <= 0) {
            throw invalid_argument("Quantity must be positive!");
        }
        
        auto item = menu.getItem(category, choice);
        order.addItem(move(item), quantity);
        
        cout << "\nItem added to your order!\n";
    }
};

int main() {
    FastFoodApp app;
    app.run();
    return 0;
}
