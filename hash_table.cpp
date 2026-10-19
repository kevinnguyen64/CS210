#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

uint64_t demoHash(const std::string& s) {
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s) {
        h = (h ^ c) * 1099511628211ULL;
    }
    return h;
}

struct UserRecord {
    std::string salt;
    uint64_t passwordHash;
};

enum class Strategy {
    Chaining,
    LinearProbing,
    QuadraticProbing,
    DoubleHashing
};

class HashTable {
private:
    using Entry = std::pair<std::string, UserRecord>;

    struct Slot {
        std::optional<Entry> entry;
        bool deleted = false;
    };

    Strategy strategy;
    size_t capacity;
    size_t count = 0;
    size_t tombstones = 0;

    std::vector<std::vector<Entry>> buckets;
    std::vector<Slot> slots;

    static bool isPrime(size_t n) {
        if (n < 2) return false;

        for (size_t d = 2; d <= n / d; ++d) {
            if (n % d == 0) return false;
        }

        return true;
    }

    static size_t nextPrime(size_t n) {
        while (!isPrime(n)) ++n;
        return n;
    }

    bool usesChaining() const {
        return strategy == Strategy::Chaining;
    }

    size_t probe(const std::string& key, size_t i) const {
        size_t start = demoHash(key) % capacity;

        if (strategy == Strategy::LinearProbing) {
            return (start + i) % capacity;
        }

        if (strategy == Strategy::QuadraticProbing) {
            return (start + i * i) % capacity;
        }

        // Prime capacity ensures this step visits every slot.
        size_t step =
            1 + demoHash("secondary:" + key) % (capacity - 1);

        return (start + i * step) % capacity;
    }

    void allocate() {
        if (usesChaining()) {
            buckets.assign(capacity, {});
        } else {
            slots.assign(capacity, {});
        }
    }

    void place(const Entry& entry) {
        if (usesChaining()) {
            buckets[demoHash(entry.first) % capacity].push_back(entry);
        } else {
            for (size_t i = 0; i < capacity; ++i) {
                Slot& slot = slots[probe(entry.first, i)];

                if (!slot.entry) {
                    if (slot.deleted) --tombstones;

                    slot.entry = entry;
                    slot.deleted = false;
                    break;
                }
            }
        }

        ++count;
    }

    void rehash(size_t newCapacity) {
        std::vector<Entry> entries;

        if (usesChaining()) {
            for (const auto& bucket : buckets) {
                for (const auto& entry : bucket) {
                    entries.push_back(entry);
                }
            }
        } else {
            for (const auto& slot : slots) {
                if (slot.entry) entries.push_back(*slot.entry);
            }
        }

        size_t oldCapacity = capacity;
        capacity = nextPrime(newCapacity);
        count = 0;
        tombstones = 0;

        allocate();

        for (const auto& entry : entries) {
            place(entry);
        }

        std::cout << "Rehash: " << oldCapacity
                  << " -> " << capacity << " buckets\n";
    }

public:
    explicit HashTable(Strategy chosen, size_t initialCapacity = 5)
        : strategy(chosen),
          capacity(nextPrime(initialCapacity < 5 ? 5 : initialCapacity)) {
        allocate();
    }

    UserRecord* find(const std::string& username) {
        if (usesChaining()) {
            auto& bucket = buckets[demoHash(username) % capacity];

            for (auto& entry : bucket) {
                if (entry.first == username) {
                    return &entry.second;
                }
            }
        } else {
            for (size_t i = 0; i < capacity; ++i) {
                Slot& slot = slots[probe(username, i)];

                if (slot.entry) {
                    if (slot.entry->first == username) {
                        return &slot.entry->second;
                    }
                } else if (!slot.deleted) {
                    return nullptr;
                }
                // Continue searching past deleted slots.
            }
        }

        return nullptr;
    }

    // Returns true for a new user, false for an updated user.
    bool insert(const std::string& username, const UserRecord& record) {
        if (UserRecord* existing = find(username)) {
            *existing = record;
            return false;
        }

        // Quadratic probing requires occupancy below 0.5.
        // Use 0.45 for all probing strategies for simplicity.
        double limit = usesChaining() ? 0.75 : 0.45;

        if (static_cast<double>(count + 1) / capacity > limit) {
            rehash(capacity * 2);
        } else if (!usesChaining() &&
                   static_cast<double>(count + tombstones + 1)
                       / capacity > limit) {
            rehash(capacity); // Clean up tombstones.
        }

        place({username, record});
        return true;
    }

    bool remove(const std::string& username) {
        if (usesChaining()) {
            auto& bucket = buckets[demoHash(username) % capacity];

            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == username) {
                    bucket.erase(it);
                    --count;
                    return true;
                }
            }
        } else {
            for (size_t i = 0; i < capacity; ++i) {
                Slot& slot = slots[probe(username, i)];

                if (slot.entry) {
                    if (slot.entry->first == username) {
                        slot.entry.reset();
                        slot.deleted = true;
                        --count;
                        ++tombstones;
                        return true;
                    }
                } else if (!slot.deleted) {
                    return false;
                }
            }
        }

        return false;
    }

    double loadFactor() const {
        return static_cast<double>(count) / capacity;
    }
};

// Demonstration only: real applications must protect the pepper.
const std::string PEPPER = "classroom-only-pepper";

UserRecord createRecord(const std::string& salt,
                        const std::string& password) {
    return {salt, demoHash(salt + password + PEPPER)};
}

void login(HashTable& users,
           const std::string& username,
           const std::string& password) {
    UserRecord* record = users.find(username);

    if (!record) {
        std::cout << username << ": unknown user\n";
        return;
    }

    uint64_t submittedHash =
        demoHash(record->salt + password + PEPPER);

    if (submittedHash == record->passwordHash) {
        std::cout << username << ": login successful\n";
    } else {
        std::cout << username << ": incorrect password\n";
    }
}

void demonstrate(Strategy strategy, const std::string& name) {
    std::cout << "\n--- " << name << " ---\n";

    HashTable users(strategy);

    users.insert("alice", createRecord("salt-a", "apple123"));
    users.insert("bob",   createRecord("salt-b", "banana456"));
    users.insert("carol", createRecord("salt-c", "cherry789"));
    users.insert("dave",  createRecord("salt-d", "orange123"));
    users.insert("eve",   createRecord("salt-e", "grape456"));

    std::cout << "Load factor: " << users.loadFactor() << '\n';

    login(users, "alice", "apple123"); // Successful login.
    login(users, "alice", "wrong");    // Failed password.
    login(users, "nobody", "test");    // Unknown user.

    if (users.remove("bob")) {
        std::cout << "Bob removed\n";
    }

    login(users, "bob", "banana456");  // Removed user.
}

int main() {
    demonstrate(Strategy::Chaining, "Separate chaining");
    demonstrate(Strategy::LinearProbing, "Linear probing");
    demonstrate(Strategy::QuadraticProbing, "Quadratic probing");
    demonstrate(Strategy::DoubleHashing, "Double hashing");

    std::cout
        << "\nSelected strategy: separate chaining.\n"
        << "It provides simple deletion, needs no tombstones,\n"
        << "and supports higher load factors.\n";

    return 0;
}
