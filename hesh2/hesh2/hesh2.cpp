#include <iostream>
#include <clocale>
using namespace std;

template <typename K, typename V>
class OpenAddressingHashTable {
private:
    enum class EntryState {
        EMPTY,
        OCCUPIED,
        DELETED
    };

    struct Entry {
        K key;
        V value;
        EntryState state;

        Entry() : state(EntryState::EMPTY) {}
        Entry(const K& k, const V& v, EntryState s = EntryState::OCCUPIED)
            : key(k), value(v), state(s) {
        }
    };

    Entry* table;
    size_t capacity;
    size_t size;
    const double load_factor_threshold = 0.7;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    void rehash() {
        size_t new_capacity = capacity * 2;
        Entry* new_table = new Entry[new_capacity];

        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == EntryState::OCCUPIED) {
                size_t index = std::hash<K>{}(table[i].key) % new_capacity;
                while (new_table[index].state == EntryState::OCCUPIED) {
                    index = (index + 1) % new_capacity;
                }
                new_table[index] = Entry(table[i].key, table[i].value, EntryState::OCCUPIED);
            }
        }

        delete[] table;
        table = new_table;
        capacity = new_capacity;
    }

public:
    OpenAddressingHashTable(size_t initial_capacity = 8)
        : capacity(initial_capacity), size(0) {
        table = new Entry[capacity];
    }

    ~OpenAddressingHashTable() {
        delete[] table;
    }

    void insert(const K& key, const V& value) {
        if (static_cast<double>(size) / capacity >= load_factor_threshold) {
            rehash();
        }

        size_t index = hash(key);
        size_t start_index = index;
        size_t first_deleted = -1;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].value = value;
                return;
            }

            if (table[index].state == EntryState::DELETED && first_deleted == static_cast<size_t>(-1)) {
                first_deleted = index;
            }

            if (table[index].state != EntryState::OCCUPIED) {
                if (first_deleted != static_cast<size_t>(-1)) {
                    index = first_deleted;
                }
                table[index] = Entry(key, value, EntryState::OCCUPIED);
                size++;
                return;
            }

            index = (index + 1) % capacity;
        } while (index != start_index);

        if (first_deleted != static_cast<size_t>(-1)) {
            table[first_deleted] = Entry(key, value, EntryState::OCCUPIED);
            size++;
        }
    }

    bool search(const K& key, V& result) const {
        size_t index = hash(key);
        size_t start_index = index;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                result = table[index].value;
                return true;
            }

            if (table[index].state == EntryState::EMPTY) {
                return false;
            }

            index = (index + 1) % capacity;
        } while (index != start_index);

        return false;
    }

    bool remove(const K& key) {
        size_t index = hash(key);
        size_t start_index = index;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].state = EntryState::DELETED;
                size--;
                return true;
            }

            if (table[index].state == EntryState::EMPTY) {
                return false;
            }

            index = (index + 1) % capacity;
        } while (index != start_index);

        return false;
    }

    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == EntryState::OCCUPIED) {
                cout << "[" << i << "]: " << table[i].key << " => " << table[i].value << endl;
            }
            else if (table[i].state == EntryState::DELETED) {
                cout << "[" << i << "]: <deleted>" << endl;
            }
            else {
                cout << "[" << i << "]: <empty>" << endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "russian");
    OpenAddressingHashTable<string, int> hash_table;

    hash_table.insert("one", 1);
    hash_table.insert("two", 2);
    hash_table.insert("three", 3);

    int value;
    if (hash_table.search("two", value)) {
        cout << "Найдено: " << value << endl; 
    }
    else {
        cout << "Не найдено" << endl;
    }

    hash_table.remove("two");
    if (!hash_table.search("two", value)) {
        cout << "Ключ 'two' удален" << endl; 
    }

    return 0;
}