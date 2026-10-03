#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

struct Property {
    std::string name;
    int cost;
    std::string owner;

    Property(std::string n = "", int c = 0) {
        name = n;
        cost = c;
        owner = "None";
    }
};

struct Node {
    Property data;
    Node* next;

    Node(Property p) {
        data = p;
        next = nullptr;
    }
};

class LinkList {
private:
    Node* head;
    Node* tail;

public:
    LinkList() {
        head = nullptr;
        tail = nullptr;
    }

    ~LinkList() {
        if (head == nullptr) {
            return;
        }

        Node* current = head->next;

        while (current != head) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }

        delete head;
    }

    void add(std::string name, int cost) {
        Node* newNode = new Node(Property(name, cost));

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            tail->next = head;
        }
        else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }
    }

    Node* search(std::string name) {
        if (head == nullptr) {
            return nullptr;
        }

        Node* current = head;

        do {
            if (current->data.name == name) {
                return current;
            }

            current = current->next;
        } while (current != head);

        return nullptr;
    }

    bool remove(std::string name) {
        if (head == nullptr) {
            return false;
        }

        Node* current = head;
        Node* previous = tail;

        do {
            if (current->data.name == name) {

                if (current == head && current == tail) {
                    delete current;
                    head = nullptr;
                    tail = nullptr;
                }
                else if (current == head) {
                    head = head->next;
                    tail->next = head;
                    delete current;
                }
                else {
                    previous->next = current->next;

                    if (current == tail) {
                        tail = previous;
                    }

                    delete current;
                }

                return true;
            }

            previous = current;
            current = current->next;
        } while (current != head);

        return false;
    }

    void print() {
        if (head == nullptr) {
            return;
        }

        Node* current = head;

        do {
            std::cout << current->data.name << " | $" << current->data.cost << " | Owner: " << current->data.owner << std::endl;
            current = current->next;
        } while (current != head);
    }

    Node* getHead() {
        return head;
    }
};

struct Player {
    std::string name;
    int money;
    Node* position;

    Player(std::string n, Node* p) {
        name = n;
        money = 1500;
        position = p;
    }
};

int rollDice() {
    return std::rand() % 6 + 1;
}

void movePlayer(Player& player, int spaces) {
    for (int i = 0; i < spaces; i++) {
        player.position = player.position->next;
    }
}

void playTurn(Player& player) {
    int roll = rollDice();

    std::cout << "\n" << player.name << " rolled " << roll << std::endl;

    movePlayer(player, roll);

    Property& property = player.position->data;

    std::cout << player.name << " landed on " << property.name << std::endl;

    if (property.owner == "None") {
        if (player.money >= property.cost) {
            player.money -= property.cost;
            property.owner = player.name;

            std::cout << player.name << " bought " << property.name << " for $" << property.cost << std::endl;
        }
    }
    else {
        std::cout << property.name << " is already owned by " << property.owner << std::endl;
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    LinkList board;

    board.add("La Jolla", 60);
    board.add("Del Mar", 60);
    board.add("Chula Vista", 100);
    board.add("North Park", 100);
    board.add("South Park", 120);
    board.add("Mission Valley", 140);
    board.add("Pacific Beach", 140);
    board.add("Ocean Beach", 160);
    board.add("Downtown San Diego", 180);
    board.add("Coronado", 180);

    std::cout << "SAN DIEGO MONOPOLY BOARD\n\n";

    board.print();

    std::cout << "\nSEARCH TEST\n";

    Node* found = board.search("Pacific Beach");

    if (found != nullptr) {
        std::cout << found->data.name << " was found." << std::endl;
    }
    else {
        std::cout << "Property was not found." << std::endl;
    }

    board.add("Test Property", 500);

    std::cout << "\nTest Property added." << std::endl;

    if (board.remove("Test Property")) {
        std::cout << "Test Property removed." << std::endl;
    }

    Player player1("Kevin", board.getHead());
    Player player2("Player 2", board.getHead());

    std::cout << "\nSTART GAME\n";

    for (int turn = 1; turn <= 10; turn++) {
        std::cout << "\nTurn " << turn << std::endl;

        if (turn % 2 == 1) {
            playTurn(player1);
        }
        else {
            playTurn(player2);
        }
    }

    std::cout << "\nFINAL BOARD\n\n";

    board.print();

    return 0;
}