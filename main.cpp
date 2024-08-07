#include <bits/stdc++.h>

template<typename KeyType, typename ValueType, typename Hash1 = std::hash<KeyType>, typename Hash2 = std::hash<KeyType>>
class CuckooHashTable {
private:
    size_t table_size_ = 10;  // Initial capacity
    static const int32_t kMaxTries = 7;
    std::vector<std::pair<KeyType, ValueType>> first_table_;
    std::vector<std::pair<KeyType, ValueType>> second_table_;
    int32_t count_elements_;
    Hash1 hash_function1_;
    Hash2 hash_function2_;

    [[nodiscard]] int32_t HashFunction1(const KeyType &key) const {
        return hash_function1_(key) % table_size_;
    }

    [[nodiscard]] int32_t HashFunction2(const KeyType &key) const {
        return hash_function2_(key) % table_size_;
    }

    void InsertInternal(KeyType key, ValueType value, int32_t table_index) {
        if (Search(key)) {
            return;
        }

        for (int32_t i = 0; i < kMaxTries; ++i) {
            int32_t hash;
            if (table_index == 1) {
                hash = HashFunction1(key);
                if (first_table_[hash].first == KeyType()) {
                    first_table_[hash] = {key, value};
                    count_elements_++;
                    return;
                } else {
                    std::swap(key, first_table_[hash].first);
                    std::swap(value, first_table_[hash].second);
                    table_index = 2;
                }
            } else {
                hash = HashFunction2(key);
                if (second_table_[hash].first == KeyType()) {
                    second_table_[hash] = {key, value};
                    count_elements_++;
                    return;
                } else {
                    std::swap(key, second_table_[hash].first);
                    std::swap(value, second_table_[hash].second);
                    table_index = 1;
                }
            }
        }

        Rehash();
        InsertInternal(key, value, 1);
    }

    [[nodiscard]] bool Search(const KeyType &key) const {
        int32_t hash1 = HashFunction1(key);
        int32_t hash2 = HashFunction2(key);

        return (first_table_[hash1].first == key) ||
               (second_table_[hash2].first == key);
    }

    bool Remove(const KeyType &key) {
        int32_t hash1 = HashFunction1(key);
        int32_t hash2 = HashFunction2(key);

        if (first_table_[hash1].first == key) {
            first_table_[hash1].first = KeyType();
            count_elements_--;
            return true;
        } else if (second_table_[hash2].first == key) {
            second_table_[hash2].first = KeyType();
            count_elements_--;
            return true;
        }

        return false;
    }

    void Rehash() {
        auto old_first_table = std::move(first_table_);
        auto old_second_table = std::move(second_table_);

        table_size_ *= 2;
        first_table_ = std::vector<std::pair<KeyType, ValueType>>(table_size_, {KeyType(), ValueType()});
        second_table_ = std::vector<std::pair<KeyType, ValueType>>(table_size_, {KeyType(), ValueType()});

        count_elements_ = 0;

        for (const auto &pair: old_first_table) {
            if (pair.first != KeyType()) {
                InsertInternal(pair.first, pair.second, 1);
            }
        }

        for (const auto &pair: old_second_table) {
            if (pair.first != KeyType()) {
                InsertInternal(pair.first, pair.second, 1);
            }
        }
    }

public:
    CuckooHashTable(Hash1 hash1 = Hash1(), Hash2 hash2 = Hash2())
            : first_table_(table_size_, {KeyType(), ValueType()}),
              second_table_(table_size_, {KeyType(), ValueType()}),
              count_elements_(0),
              hash_function1_(hash1),
              hash_function2_(hash2) {}

    void Insert(const KeyType &key, const ValueType &value) {
        InsertInternal(key, value, 1);
    }

    size_t TableSize() const {
        return count_elements_;
    }

    bool SearchKey(const KeyType &key) const {
        return Search(key);
    }

    bool RemoveKey(const KeyType &key) {
        return Remove(key);
    }
};

int main() {
    CuckooHashTable<int, std::string> hash_table;
    return 0;
}