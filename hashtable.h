#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <stdexcept>

template <class K, class V>
class HashTable
{
public:
    HashTable();
    ~HashTable();
    HashTable(HashTable& b);
    HashTable(const HashTable& b);
    HashTable(HashTable&& b) noexcept;

    HashTable& operator=(const HashTable& b);
    HashTable& operator=(HashTable&& b) noexcept;

    void swap(HashTable& b);  

    void clear();  

    bool erase(const K& k);  

    bool insert(const K& k, const V& v);  

    bool contains(const K& k) const;  

    size_t size() const;  
    bool empty() const;  

    
    V& operator[](const K& k);

    
    V& at(const K& k);
    const V& at(const K& k) const;

    template <class Ks, class Vs>
    friend bool operator==(const HashTable<Ks, Vs>& a, const HashTable<Ks, Vs>& b);

    template <class Ks, class Vs>
    friend bool operator!=(const HashTable<Ks, Vs>& a, const HashTable<Ks, Vs>& b);

private:
    std::vector<std::list<std::pair<K, V>>> *_table; 
    size_t _table_size;
    size_t _number_elements;
    const double _load_factor = 0.75;

    size_t _hash(const K& key, size_t size) const;
    void _rehash();
    V& _get_value(const K& k) const;
};
