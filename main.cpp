#include <bits/stdc++.h>

class CuckooHashTable {
private:
    size_t TABLE_SIZE = 10; // start capacity
    static const int MAX_TRIES = 7;
    std::vector<int> firstTable;
    std::vector<int> secondTable;
    int countElements;

    int hashFunction1(int key) const {
        return key % TABLE_SIZE;
    }

    int hashFunction2(int key) const {
        return (key / TABLE_SIZE) % TABLE_SIZE;
    }

    void insert(int key, int tableIndex) {
        if (search(key)) {
            return;
        }

        for (int i = 0; i < MAX_TRIES; ++i) {
            int hash;
            if (tableIndex == 1) {
                hash = hashFunction1(key);
                if (firstTable[hash] == INT32_MIN) {
                    firstTable[hash] = key;
                    countElements++;
                    return;
                } else {
                    std::swap(key, firstTable[hash]);
                    tableIndex = 2;
                }
            } else {
                hash = hashFunction2(key);
                if (secondTable[hash] == INT32_MIN) {
                    secondTable[hash] = key;
                    countElements++;
                    return;
                } else {
                    std::swap(key, secondTable[hash]);
                    tableIndex = 1;
                }
            }
        }

        rehash();
        insert(key, 1);
    }

    void insert(int key) {
        insert(key, 1);
    }

    bool search(int key) const {
        int hash1 = hashFunction1(key);
        int hash2 = hashFunction2(key);

        return (firstTable[hash1] == key) || (secondTable[hash2] == key);
    }

    bool remove(int key) {
        int hash1 = hashFunction1(key);
        int hash2 = hashFunction2(key);

        if (firstTable[hash1] == key) {
            firstTable[hash1] = INT32_MIN;
            countElements--;
            return true;
        } else if (secondTable[hash2] == key) {
            secondTable[hash2] = INT32_MIN;
            countElements--;
            return true;
        }

        return false;
    }

    void rehash() {
        std::vector<int> oldfirstTable = std::move(firstTable);
        std::vector<int> oldsecondTable = std::move(secondTable);

        TABLE_SIZE *= 2;
        firstTable = std::vector<int>(TABLE_SIZE, INT32_MIN);
        secondTable = std::vector<int>(TABLE_SIZE, INT32_MIN);

        countElements = 0;

        for (int key: oldfirstTable) {
            if (key != INT32_MIN) {
                insert(key, 1);
            }
        }

        for (int key: oldsecondTable) {
            if (key != INT32_MIN) {
                insert(key, 1);
            }
        }
    }

public:
    CuckooHashTable() : firstTable(TABLE_SIZE, INT32_MIN), secondTable(TABLE_SIZE, INT32_MIN), countElements(0) {}

    void insertKey(int key) {
        insert(key);
    }

    size_t tableSize() const {
        return countElements;
    }

    void searchKey(int key) const {
        if (search(key)) {
            std::cout << "Key " << key << " found in the table." << std::endl;
        } else {
            std::cout << "Key " << key << " not found in the table." << std::endl;
        }
    }

    void removeKey(int key) {
        if (remove(key)) {
            std::cout << "Key " << key << " removed from the table." << std::endl;
        } else {
            std::cout << "Key " << key << " not found in the table." << std::endl;
        }
    }

    void printTable() const {
        std::cout << "1st table: ";
        for (int el: firstTable) {
            if (el != INT32_MIN) {
                std::cout << el << " ";
            }
        }
        std::cout << "\n" << "2nd table: ";
        for (int el: secondTable) {
            if (el != INT32_MIN) {
                std::cout << el << " ";
            }
        }
        std::cout << "\n";
    }
};

int main() {
    CuckooHashTable hashTable;

    hashTable.insertKey(10);
    hashTable.printTable();
    hashTable.insertKey(100);
    hashTable.printTable();
    hashTable.insertKey(0);
    hashTable.printTable();
    hashTable.insertKey(150);
    hashTable.printTable();
    hashTable.removeKey(150);
    hashTable.printTable();
    hashTable.searchKey(10);
    hashTable.insertKey(11);
    hashTable.insertKey(165);
    hashTable.insertKey(1213);
    hashTable.insertKey(165);
    hashTable.insertKey(78);
    hashTable.insertKey(90);
    hashTable.insertKey(3567);
    hashTable.insertKey(8989);
    hashTable.insertKey(90879);
    hashTable.insertKey(0);
    hashTable.insertKey(10);
    hashTable.insertKey(100);
    hashTable.insertKey(1000);
    hashTable.insertKey(10000);
    hashTable.insertKey(100000);
    hashTable.insertKey(1000000);
    hashTable.printTable();

    std::cout << "Table size: " << hashTable.tableSize() << std::endl;

    return 0;
}
