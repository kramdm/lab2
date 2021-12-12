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


template <class K, class V>
bool operator==(const HashTable<K, V>& a, const HashTable<K, V>& b);

template <class K, class V>
bool operator!=(const HashTable<K, V>& a, const HashTable<K, V>& b);


template <class K, class V>
HashTable<K, V>::HashTable()
{
    _table_size = 32;
    _number_elements = 0;
    _table = new std::vector<std::list<std::pair<K, V>>>(_table_size);
}

template <class K, class V>
HashTable<K, V>::~HashTable()
{
    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i).clear();
    }
    delete _table;
}

template <class K, class V>
HashTable<K, V>::HashTable(HashTable &b)
{
    _table_size = b._table_size;
    _number_elements = b._number_elements;
    _table = new std::vector<std::list<std::pair<K, V>>>(_table_size);

    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i) = b._table->at(i);
    }
}


template <class K, class V>
HashTable<K, V>::HashTable(const HashTable &b)
{
    _table_size = b._table_size;
    _number_elements = b._number_elements;
    _table = new std::vector<std::list<std::pair<K, V>>>(_table_size);

    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i) = b._table->at(i);
    }
}

template <class K, class V>
HashTable<K, V>::HashTable(HashTable &&b) noexcept
{
    _table_size = b._table_size;
    _number_elements = b._number_elements;
    _table = b._table;

    b._table = nullptr;
    b._table_size = 0;
    b._number_elements = 0;
}

template <class K, class V>
HashTable<K, V> &HashTable<K, V>::operator=(const HashTable &b)
{
    if (this == &b)
        return *this;

    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i).clear();
    }
    delete _table;

    _table_size = b._table_size;
    _number_elements = b._number_elements;

    _table = new std::vector<std::list<std::pair<K, V>>>(_table_size);
    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i) = b._table->at(i);
    }

    return *this;
}

template <class K, class V>
HashTable<K, V> &HashTable<K, V>::operator=(HashTable &&b) noexcept
{
    if (this == &b)
        return *this;

    _table_size = b._table_size;
    _number_elements = b._number_elements;
    _table = b._table;

    b._table = nullptr;
    b._table_size = 0;
    b._number_elements = 0;

    return *this;
}

template <class K, class V>
void HashTable<K, V>::swap(HashTable &b)
{
    if (this == &b)
        return;

    std::swap(_table_size, b._table_size);
    std::swap(_number_elements, b._number_elements);

    std::vector<std::list<std::pair<K, V>>> *tmp = _table;
    _table = b._table;
    b._table = tmp;
}

template <class K, class V>
void HashTable<K, V>::clear()
{
    for (unsigned int i = 0; i < _table_size; ++i)
    {
        _table->at(i).clear();
    }
    _table_size = 0;
    _number_elements = 0;

    _table->clear();
}

template <class K, class V>
bool HashTable<K, V>::erase(const K& k)
{
    size_t idx = _hash(k, _table_size);

    for (auto it = _table->at(idx).begin(); it != _table->at(idx).end(); ++it)
    {
        if (it->first == k)
        {
            _table->at(idx).erase(it);
            --_number_elements;
            return true;
        }
    }
    return false;
}

template <class K, class V>
bool HashTable<K, V>::insert(const K& k, const V& v)
{
    if (contains(k))
        return false;

    if ((size_t)(_table_size * _load_factor) <= _number_elements)
        _rehash();

    size_t idx = _hash(k, _table_size);
    _table->at(idx).push_back({k, v});

    ++_number_elements;

    return true;
}

template <class K, class V>
bool HashTable<K, V>::contains(const K& k) const
{
    size_t idx = _hash(k, _table_size);

    for (auto & it : _table->at(idx))
    {
        if (it.first == k)
            return true;
    }
    return false;
}

template <class K, class V>
size_t HashTable<K, V>::size() const
{
    return _number_elements;
}

template <typename K, class V>
bool HashTable<K, V>::empty() const
{
    return (_number_elements == 0);
}

template <class K, class V>
V& HashTable<K, V>::operator[](const K& k)
{
    if (!contains(k))
    {
        insert(k, {0 , 0});
    }

    return _get_value(k);
}

template <class K, class V>
V& HashTable<K, V>::at(const K& k)
{
    if (!contains(k))
    {
        throw std::out_of_range("this key is not in hash table");
    }

    return _get_value(k);
}

template <class K, class V>
const V& HashTable<K, V>::at(const K& k) const
{
    if (!contains(k))
    {
        throw std::out_of_range("this key is not in hash table");
    }

    return _get_value(k);
}

template <class K, class V>
bool operator==(const HashTable<K, V>& a, const HashTable<K, V>& b)
{
    if (a._table_size != b._table_size || a._number_elements != b._number_elements)
        return false;

    for (size_t idx = 0; idx < a._table_size; ++idx)
    {
        if (a._table->at(idx).size() != b._table->at(idx).size())
            return false;

        auto a_it = a._table->at(idx).begin();
        auto b_it = b._table->at(idx).begin();

        for (; a_it != a._table->at(idx).end(); ++a_it, ++b_it)
        {
            if (a_it->first != b_it->first || a_it->second != b_it->second)
                return false;
        }
    }
    return true;
}

template <class K, class V>
bool operator!=(const HashTable<K, V>& a, const HashTable<K, V>& b)
{
    return !(a == b);
}

template <typename K, class V>
size_t HashTable<K, V>::_hash(const K& key, size_t size) const
{
    return (std::hash<K>{}(key) % size);
}

template <class K, class V>
void HashTable<K, V>::_rehash()
{
    size_t new_size = _table_size * 2;
    std::vector<std::list<std::pair<K, V>>> *new_table;
    new_table = new std::vector<std::list<std::pair<K, V>>>(new_size);

    for (size_t cell_idx = 0; cell_idx != _table_size; ++cell_idx)
    {
        for (auto &it : _table->at(cell_idx))
        {
            size_t idx = _hash(it.first, new_size);
            new_table->at(idx).push_back({it.first, it.second});
        }
        _table->at(cell_idx).clear();
    }
    _table->clear();
    _table = new_table;

    _table_size = new_size;
}

template <class K, class V>
V& HashTable<K, V>::_get_value(const K& k) const
{
    size_t idx = _hash(k, _table_size);
    for (auto & it : _table->at(idx))
    {
        if (it.first == k)
            return it.second;
    }
}
