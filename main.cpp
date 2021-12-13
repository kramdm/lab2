#include "hashtable.h"

struct Student
{
    size_t age;
    size_t weight;

    friend bool operator==(const Student& a, const Student& b)
    {
        if (a.age == b.age && a.weight == b.weight)
            return true;
        return false;
    }

    friend bool operator!=(const Student& a, const Student& b)
    {
        return !(a == b);
    }
};

typedef std::string Key;
typedef Student Value;


using namespace std;

class TestHashTable : public::testing::Test
{
protected:
    HashTable<Key, Value> table;

    void SetUp() override
    {
        table.insert("Max", {18, 50});
        table.insert("Vanya", {18, 76});
        table.insert("Misha", {18, 80});

        string chars = "qwertyuiopasdfghjklzxcvbnm";
        for (auto i : chars)
        {
            string x(1, i);
            table.insert(x, {1, 1});
        }
    }

    void TearDown() override {}
};

TEST_F(TestHashTable, TestContains)
{
    ASSERT_TRUE(table.contains("Max"));
    ASSERT_TRUE(table.contains("Misha"));
    ASSERT_TRUE(table.contains("Vanya"));
    ASSERT_TRUE(table.contains("a"));
    ASSERT_FALSE(table.contains("Timur"));
    ASSERT_FALSE(table.contains("Roy"));
    ASSERT_FALSE(table.contains("qwerty"));
}

TEST_F(TestHashTable, TestSize)
{
    ASSERT_EQ(table.size(), 29);

    table.erase("Max");

    ASSERT_EQ(table.size(), 28);

    table.clear();

    ASSERT_EQ(table.size(), 0);
}

TEST_F(TestHashTable, TestEmptyAndClear)
{
    ASSERT_FALSE(table.empty());
    table.clear();
    ASSERT_TRUE(table.empty());
}

TEST_F(TestHashTable, TestErase)
{
    ASSERT_FALSE(table.empty());
    ASSERT_EQ(table.size(), 29);

    ASSERT_FALSE(table.erase("qwerty"));
    ASSERT_EQ(table.size(), 29);

    ASSERT_TRUE(table.erase("Max"));
    ASSERT_TRUE(table.erase("Vanya"));
    ASSERT_TRUE(table.erase("Misha"));

    ASSERT_EQ(table.size(), 26);

    string chars = "qwertyuiopasdfghjklzxcvbnm";
    for (auto i : chars)
    {
        string x(1, i);
        ASSERT_TRUE(table.erase(x));
    }
    ASSERT_EQ(table.size(), 0);
    ASSERT_TRUE(table.empty());

    ASSERT_FALSE(table.erase("qwerty"));
    ASSERT_EQ(table.size(), 0);
    ASSERT_TRUE(table.empty());
}

TEST_F(TestHashTable, TestInsert)
{
    ASSERT_EQ(table.size(), 29);

    ASSERT_TRUE(table.insert("qwerty", {50, 50}));
    ASSERT_EQ(table.size(), 30);

    ASSERT_FALSE(table.insert("Max", {18, 60}));
    ASSERT_EQ(table.size(), 30);
}

TEST_F(TestHashTable, TestSwap)
{
    HashTable<Key, Value> new_table;
    new_table.insert("qwerty", {13, 13});

    ASSERT_EQ(new_table.size(), 1);

    new_table.swap(table);

    ASSERT_EQ(new_table.size(), 29);
    ASSERT_TRUE(new_table.contains("Max"));
    ASSERT_TRUE(new_table.contains("Misha"));
    ASSERT_TRUE(new_table.contains("Vanya"));
    ASSERT_TRUE(new_table.contains("a"));
    ASSERT_FALSE(new_table.contains("Timur"));
    ASSERT_FALSE(new_table.contains("Roy"));
    ASSERT_FALSE(new_table.contains("qwerty"));

    ASSERT_EQ(table.size(), 1);
    ASSERT_TRUE(table.contains("qwerty"));
    ASSERT_FALSE(table.contains("Max"));
    ASSERT_FALSE(table.contains("Misha"));
    ASSERT_FALSE(table.contains("Vanya"));
}

TEST_F(TestHashTable, TestSquareBrackets)
{
    Value test_value = {18, 50};
    ASSERT_EQ(table["Max"], test_value);

    test_value = {18, 80};
    ASSERT_EQ(table["Misha"], test_value);

    test_value = {1, 1};
    ASSERT_EQ(table["q"], test_value);

    test_value = {0, 0};
    ASSERT_EQ(table["qwerty"], test_value);
}

TEST_F(TestHashTable, TestAt)
{
    Value test_value = {18, 50};
    ASSERT_EQ(table.at("Max"), test_value);

    test_value = {18, 80};
    ASSERT_EQ(table.at("Misha"), test_value);

    test_value = {1, 1};
    ASSERT_EQ(table.at("q"), test_value);

    ASSERT_THROW(table.at("qwerty"), std::out_of_range);
}

TEST_F(TestHashTable, TestCompareOperators)
{
    HashTable<Key, Value> new_table(table);
    ASSERT_TRUE(table == new_table);

    new_table.erase("Max");
    ASSERT_FALSE(table == new_table);

    
}

TEST_F(TestHashTable, TestMove)
{
    Value test_value = {18, 50};

    HashTable<Key, Value> b;
    b = std::move(table);

    ASSERT_EQ(table.size(), 0);
    ASSERT_EQ(b.at("Max"), test_value);

    HashTable<Key, Value> c(std::move(b));
    ASSERT_EQ(b.size(), 0);
    ASSERT_EQ(c.at("Max"), test_value);
}
