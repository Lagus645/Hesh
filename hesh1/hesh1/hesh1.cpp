#include <iostream>
#include <clocale>
using namespace std;

template <typename K, typename V>
class HashTable {
private:
    static const int HASH_GROUPS = 10; // Количество бакетов

    // Узел односвязного списка
    struct Node {
        K key;
        V value;
        Node* next;

        Node(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[HASH_GROUPS] = { nullptr }; // Массив указателей на списки

    // Хэш-функция
    size_t hashFunction(K key) {
        return std::hash<K>()(key) % HASH_GROUPS;
    }

public:
    ~HashTable() {
        // Очистка памяти при удалении таблицы
        for (int i = 0; i < HASH_GROUPS; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Вставка элемента
    void insert(K key, V value) {
        size_t hashValue = hashFunction(key);
        Node*& head = table[hashValue];

        // Проверяем, есть ли уже такой ключ
        Node* current = head;
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // Обновляем значение
                return;
            }
            current = current->next;
        }

        // Вставляем новый узел в начало списка
        Node* newNode = new Node(key, value);
        newNode->next = head;
        head = newNode;
    }

    // Поиск элемента
    bool find(K key, V& value) {
        size_t hashValue = hashFunction(key);
        Node* current = table[hashValue];

        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }

        return false;
    }

    // Удаление элемента
    bool remove(K key) {
        size_t hashValue = hashFunction(key);
        Node*& head = table[hashValue];
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    head = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }

        return false;
    }

    // Вывод таблицы (для отладки)
    void print() {
        for (int i = 0; i < HASH_GROUPS; ++i) {
            cout << "Bucket " << i << ": ";
            Node* current = table[i];
            while (current != nullptr) {
                cout << "[" << current->key << ": " << current->value << "] ";
                current = current->next;
            }
            cout << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "russian");
    HashTable<string, int> ht;

    // Вставка элементов
    ht.insert("one", 1);
    ht.insert("two", 2);
    ht.insert("three", 3);

    // Поиск элементов
    int value;
    if (ht.find("two", value)) {
        cout << "Найдено: two -> " << value << endl;
    }
    else {
        cout << "Не найдено" << endl;
    }

    if (ht.find("four", value)) {
        cout << "Найдено: four -> " << value << endl;
    }
    else {
        cout << "Не найдено" << endl;
    }

    // Удаление элемента
    ht.remove("two");

    // Вывод таблицы
    ht.print();

    return 0;
}