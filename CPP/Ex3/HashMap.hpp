//
// Created by Ron on 13-Sep-19.
//

#include <vector>
#include <utility>
#include <algorithm>
#include <exception>
#include <iostream>


#ifndef EX3_HASHMAP_H
#define EX3_HASHMAP_H

/**
 * @tparam KeyT keys
 * @tparam ValueT values
 */
template<typename KeyT, typename ValueT>
/**
 * A class implementing the generic hashmap, using a dynamic array of vectors containing pairs of
 * keys and values.
 * @tparam KeyT key of type KeyT
 * @tparam ValueT value of type ValueT
 */
class HashMap
{
    using pair = std::pair<KeyT, ValueT>;
    using bucket = std::vector<pair>;
    using bucketIter = typename bucket::iterator;

private:
    int _size;
    int _capacity;
    double _upperLoadFactor;
    double _lowerLoadFactor;
    bucket *_bucketsArray;

// ================================ PRIVATE HELPER METHODS =======================================


    /**
     * Performs hashing on a key.
     * @param key key of KeyT value
     * @return hashed index
     */
    int _hash(KeyT key) const
    {
        return std::hash<KeyT>()(key) & (_capacity - 1);
    }

    /**
     * @return true if the lower load factor requires resizing the table; false otherwise.
     */
    bool _checkLowerLoadFactor() const
    {
        return (getLoadFactor() < _lowerLoadFactor);
    }

    /**
     * @return true if after adding element, the upper load factor requires resizing the table;
     * false otherwise.
     */
    bool _checkUpperLoadFactor() const
    {
        return (((double) (_size + 1) / (double) _capacity) > _upperLoadFactor);
    }

    /**
     * Performs rehashing and increasing/decreasing the table;
     * @param command 1 for increase, -1 for decrease;
     */
    void _rehashManager(int command)
    {
        int oldCapacity = _capacity;
        if (command == 1)
        {
            _capacity *= 2;
        }
        if (command == -1)
        {
            _capacity /= 2;
        }
        bucket *temp;
        try
        {
            temp = new bucket[_capacity];
        }
        catch (std::bad_alloc &e)
        {
            delete[] _bucketsArray;
            throw std::bad_alloc();
        }
        for (int i = 0; i < oldCapacity; ++i)
        {
            for (pair p : _bucketsArray[i])
            {
                temp[i].push_back(p);
            }
        }
        delete[] _bucketsArray;
        _bucketsArray = temp;
    }

    /**
    * Finds the cell of the key and returns a reference to its value; assumes key is in hashmap.
    * @param key key in hashmap
    * @return reference to value
    */
    ValueT &_findCell(const KeyT &key) const
    {
        for (pair &p: _bucketsArray[_hash(key)])
        {
            if (p.first == key)
            {
                return p.second;
            }
        }
    }

public:

// ================================= CTORS, DTOR & RULE OF 5 =====================================

    /**
     * Default constructor for hash map.
     */
    HashMap() : HashMap(0.25, 0.75)
    {}

    /**
     * Constructor which receives specific lower and upper load factors;
     * @param lowerLoadFactor lower load factor
     * @param upperLoadFactor upper load factor
     */
    HashMap(double lowerLoadFactor, double upperLoadFactor) : _size(0), _capacity(16),
                                                              _upperLoadFactor(upperLoadFactor),
                                                              _lowerLoadFactor(lowerLoadFactor),
                                                              _bucketsArray(new bucket[16])
    {
        if (lowerLoadFactor >= upperLoadFactor || lowerLoadFactor <= 0 || upperLoadFactor >= 1)
        {
            delete[] _bucketsArray;
            throw std::invalid_argument("Invalid load factors");
        }
    }

    /**
     * Builds a hashmap from 2 vectors of keys and values.
     * @param keys vector containing keys
     * @param values vector containing values
     */
    HashMap(std::vector<KeyT> keys, std::vector<ValueT> values) : HashMap(0.25, 0.75)
    {
        if (keys.size() != values.size())
        {
            throw std::invalid_argument("Vectors size is not equal");
        }
        HashMap(); // create with default values
        for (size_t i = 0; i < keys.size(); ++i)
        {
            bucket &b = _bucketsArray[_hash(keys[i])];
            for (pair &p: b)
            {
                if (p.first == keys[i])
                {
                    p.second = values[i];
                }
            }
            // else, new key, do not override
            insert(keys[i], values[i]);
        }
    }

    /**
     * Copy constructor.
     * @param other other hashmap to copy values from
     */
    HashMap(const HashMap &other) : _size(0), _capacity(other._capacity),
                                    _lowerLoadFactor(other._lowerLoadFactor),
                                    _upperLoadFactor(other._upperLoadFactor),
                                    _bucketsArray(new bucket[other._capacity])
    {
        // size incremented during insert operations
        for (auto pair : other)
        {
            this->insert(pair.first, pair.second);
        }
    };

    /**
     * Copy assignment operator.
     * @param other other hashmap to assign values from.
     */
    HashMap &operator=(const HashMap &other)
    {
        if (this != &other)
        {
            if (_capacity != other._capacity)
            {
                delete[] _bucketsArray;
                _bucketsArray = new bucket[other._capacity];
            }
            else
            {
                this->clear();
            }
            _capacity = other._capacity;
            _lowerLoadFactor = other._lowerLoadFactor;
            _upperLoadFactor = other._upperLoadFactor;
            for (auto pair : other)
            {
                this->insert(pair.first, pair.second);
            }
        }
        return *this;
    }

    /**
     * Move constuctor.
     * @param other other hashmap to move values from
     */
    HashMap(HashMap &&other) noexcept : _size(0), _capacity(other._capacity),
                                        _lowerLoadFactor(other._lowerLoadFactor),
                                        _upperLoadFactor(other._upperLoadFactor),
                                        _bucketsArray(new bucket[other._capacity])
    {
        // size incremented during insert operations
        for (auto pair : other)
        {
            this->insert(pair.first, pair.second);
        }
        delete[] other._bucketsArray;
        other._bucketsArray = nullptr;
    }

    /**
     * Move assignment operator.
     * @param other other hashmap to assign values from
     */
    HashMap &operator=(HashMap &&other) noexcept
    {
        if (this != &other)
        {
            if (_capacity != other._capacity)
            {
                delete[] _bucketsArray;
                _bucketsArray = new bucket[other._capacity];
            }
            else
            {
                this->clear();
            }
            _capacity = other._capacity;
            _lowerLoadFactor = other._lowerLoadFactor;
            _upperLoadFactor = other._upperLoadFactor;
            for (auto pair : other)
            {
                this->insert(pair.first, pair.second);
            }
            delete[] other._bucketsArray;
            other._bucketsArray = nullptr;
        }
        return *this;
    }

    /**
     * Destructor.
     */
    ~HashMap() noexcept
    {
        delete[] _bucketsArray;
    }

// ======================================= API METHODS ===========================================


    /**
     * @return size of hashmap, i.e. number of elements.
     */
    int size() const
    { return _size; }

    /**
     * @return capacity of hashmap.
     */
    int capacity() const
    { return _capacity; }

    /**
     * @return current load factor of hashmap.
     */
    double getLoadFactor() const
    { return ((double) _size / (double) _capacity); }

    /**
     * @return true if hash map is empty; false otherwise.
     */
    bool empty() const
    {
        return (_size == 0);
    }


    /**
     * Insert a pair of <key,value> to the hashmap.
     * If a pair with such key already exists, does not perform insertion and returns false;
     * else, check if resizing is needed, and insert the pair.
     * @param key key of pair
     * @param value value of pair
     * @return true is insertion was performed; false otherwise.
     */
    bool insert(KeyT key, ValueT value)
    {
        if (containsKey(key))
        {
            return false;
        }
        if (_checkUpperLoadFactor())
        {
            _rehashManager(1);
        }
        _bucketsArray[_hash(key)].push_back(pair(key, value));
        _size++;
        return true;
    }

    /**
     * @param key key of KeyT
     * @return true if key exists in hashmap.
     */
    bool containsKey(KeyT key) const
    {
        for (pair &p: _bucketsArray[_hash(key)])
        {
            if (p.first == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @param key key of type KeyT
     * @return returns reference to value of the key if key exists; throws std::out_of_range
     * otherwise.
     */
    ValueT &at(const KeyT &key)
    {

        if (_bucketsArray != nullptr && containsKey(key))
        {
            return _findCell(key);
        }
        throw std::out_of_range("Invalid key");
    }

    /**
    * @param key key of type KeyT
    * @return returns const reference to value of the key if key exists; throws std::out_of_range
    * otherwise.
    */
    const ValueT &at(const KeyT &key) const
    {
        if (_bucketsArray != nullptr && containsKey(key))
        {
            return _findCell(key);
        }
        throw std::out_of_range("Invalid key");
    }

    /**
     * Removes a key from hashmap is key exists, and then checks if resizing is neccessary. if key
     * does not exist, return false;
     * @param key key of type KeyT
     * @return true if key was successfully was erased; false otherwise.
     */
    bool erase(KeyT key)
    {
        if (!containsKey(key))
        {
            return false;
        }
        for (auto it = _bucketsArray[_hash(key)].begin();
             it != _bucketsArray[_hash(key)].end(); ++it)
        {
            if (it->first == key)
            {
                _bucketsArray[_hash(key)].erase(it);
                --_size;
                break; //break after iterator is no longer valid,we only have one appearance of key
            }
        }
        if (_checkLowerLoadFactor())
        {
            _rehashManager(-1);
        }
    }

    /**
     * @param key key of type KeyT
     * @return size of bucket which key exists in if key is in hashmap; throws std::out_of_range
     * if key does not exist in hashmap.
     */
    int bucketSize(KeyT key) const
    {
        if (containsKey(key))
        {
            return _bucketsArray[_hash(key)].size();
        }
        throw std::out_of_range("Invalid key");
    }

    /**
     * clear the hashmap from all the elements.
     */
    void clear()
    {
        for (int i = 0; i < _capacity; ++i) // clear every cell
        {
            _bucketsArray[i].clear();
        }
        _size = 0;
    }

// ================================= OPERATORS OVERLOADING =======================================

    /**
     * equality comparison operator overloading.
     * @param other other hashmap to compare to
     * @return true if hashmap are identical; false otherwise;
     */
    bool operator==(const HashMap &other) const
    {
        if (_size != other._size || _capacity != other._capacity ||
            _lowerLoadFactor != other._lowerLoadFactor ||
            _upperLoadFactor != other._upperLoadFactor) // check equality of data members
        {
            return false;
        }
        for (int i = 0; i < _capacity; ++i) // check both maps key->value pairs are identical
        {
            std::sort(_bucketsArray[i].begin(), _bucketsArray[i].end());
            std::sort(other._bucketsArray[i].begin(), other._bucketsArray[i].end());
            if (_bucketsArray[i] != other._bucketsArray[i]) // use built-in vector comparison
            {
                return false;
            }
        }
        return true;
    }

    /**
    * inequality comparison operator overloading.
    * @param other other hashmap to compare to
    * @return false if hashmap are identical; true otherwise;
    */
    bool operator!=(const HashMap &other) const
    {
        return !(*this == other);
    }

    /**
     * [] operator overloading.
     * @param key key of type KeyT
     * @return reference to the value of key if key exists; else create a default ValueT and
     * returns a reference to it.
     */
    ValueT &operator[](const KeyT &key) noexcept
    {
        if (containsKey(key))
        {
            return _findCell(key);
        }
        else
        {
            ValueT defaultVal = ValueT();
            insert(key, defaultVal);
            return _findCell(key);
        }
    }

    /**
    * [] operator overloading.
    * @param key key of type KeyT
    * @return reference to the value of key if key exists; else, behaviour is undefined.
    */
    const ValueT &operator[](const KeyT &key) const noexcept
    {
        if (containsKey(key))
        {
            return _findCell(key);
        }
    }

// ======================================= ITERATOR ==============================================

    /**
     * Iterator for hashmap, allowing to perform for-each loop on hashmaps.
     */
    class iterator
    {

    private:
        const HashMap *_map; // pointer to map
        int _bucketIdx; // current bucket index
        bucket *_bucket; // pointer to current bucket
        bucketIter _cell; // iterator of the cell in current bucket


    public:
        /**
         * Constructor for iterator.
         * @param map hashmap pointer
         * @param start bool - if start returns the beginning of hashmap; else value is end, return
         * the end of hashmap.
         */
        iterator(const HashMap *map, bool start) : _map(map)
        {
            if (start)
            {
                for (int i = 0; i < map->_capacity; ++i)
                {
                    if (!map->_bucketsArray[i].empty())
                    {
                        _bucketIdx = i;
                        break;
                    }
                }
                _bucket = _map->_bucketsArray + _bucketIdx;
                _cell = _bucket->begin();
            }
            else
            {
                _bucketIdx = map->_capacity - 1;
                _bucket = _map->_bucketsArray + _bucketIdx;
                _cell = _bucket->end();
            }
        }

        /**
         * Prefix ++ operator.
         * @return this iterator
         */
        iterator operator++()
        {
            ++_cell;
            if (_cell != _bucket->end())
            {
                return *this;
            }
            // else, end of current bucket
            // continue to next bucket while buckets are empty
            while ((_bucket + 1)->empty())
            {
                ++_bucketIdx;
                ++_bucket;
            }
            // check if we are at the last bucket
            if (_bucketIdx < _map->_capacity - 1)
            {
                ++_bucketIdx;
                ++_bucket;
                _cell = _bucket->begin();
                return *this;
            }
            else
            {
                _cell = _bucket->end();
            }
            return *this;


        }

        /**
         * Post fix ++ operator.
         * @return this iterator
         */
        iterator operator++(int)
        {
            iterator temp = *this;
            ++(*this);
            return temp;

        }

        /**
         * Dereference operator.
         * @return const reference to the current pair pointed by _cell
         */
        const pair &operator*() const
        { return *_cell; }

        /**
        * Arrow operator.
        * @return reference to the current pair pointed by _cell
        */
        pair &operator->() const
        { return &(*_cell); }

        /**
         * Equality comparison operator.
         * @param rhs other iterator
         * @return true if iterators are identical; false otherwise.
         */
        bool operator==(const iterator &rhs) const
        {
            return (_map == rhs._map && _bucketIdx == rhs._bucketIdx &&
                    _cell == rhs._cell && _bucket == rhs._bucket);
        }

        /**
        * Inequality comparison operator.
        * @param rhs other iterator
        * @return false if iterators are identical; true otherwise.
        */
        bool operator!=(const iterator &rhs) const
        {
            return !(*this == rhs);
        }


    };

    typedef HashMap<KeyT, ValueT>::iterator const_iterator;

    /**
     * @return first pair of hashmap, i.e. the beginning of hashmap.
     */
    const_iterator begin() const
    {
        return const_iterator(this, true);
    }

    /**
    * @return end of hashmap, pointer to the last pair + 1.
    */
    const_iterator end() const
    {
        return const_iterator(this, false);
    }

    /**
     * @return first pair of hashmap, i.e. the beginning of hashmap.
     */
    const const_iterator cbegin() const
    {
        return const_iterator(this, true);
    }

    /**
    * @return end of hashmap, pointer to the last pair + 1.
    */
    const const_iterator cend() const
    {
        return const_iterator(this, false);
    }

};


#endif //EX3_HASHMAP_H
