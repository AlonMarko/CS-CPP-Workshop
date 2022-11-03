//
// Created by alonn on 8/4/2020.
//
#ifndef VLVECTOR_VLVECTOR_HPP
#define VLVECTOR_VLVECTOR_HPP
#define STATICCAPACITY 16

#include <iterator>

/**
 * @brief
 * @tparam T
 * @tparam capVLV
 */
template<typename T, size_t capVLV = STATICCAPACITY>
class VLVector
{
private:
    size_t _size; // how many items are currently in the vector
    size_t _capacity; // how many items can the vector hold
    T arr[capVLV * sizeof(T)]; // the static array
    T *_arr = NULL; //the heap array


public:
    // since pointers are already random access iterators we will use them for out iterators
    // instead of creating a whole new 2 classes for them. so we will just create begin and end
    // methods in this class accordingly and the data() method to get the appropriate pointer
    typedef const T *const_iterator;
    typedef T *iterator;

    /**
     * @brief the capC function - no explenation needed
     */
    size_t capC(size_t s, size_t amount = 1)
    {
        if (s + amount <= capVLV)
        {

            return capVLV;
        }
        return (size_t) ((3 * (s + amount)) / 2);
    }

    /**
     * @brief default constructor!
     */
    VLVector()
    {
        _capacity = capVLV;
        _size = 0;
    }

    /**
     * @brief  constructor 1
     * @tparam InputIterator iterator for T
     * @param first where we begin
     * @param last where we stop
     */
    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last)
    {
        size_t inputSize = 0;
        InputIterator firstCopy = first;
        while (firstCopy != last)
        {
            firstCopy++;
            inputSize++;
        }
        unsigned int counter = 0;
        _size = inputSize;
        if (_size > capVLV)
        {
            _capacity = this->capC(inputSize);
            _arr = new T[_capacity];
            while (first != last)
            {
                _arr[counter] = *first;
                counter++;
                first++;
            }
        }
        else
        {
            _capacity = capVLV;
            while (first != last)
            {
                arr[counter] = *first;
                counter++;
                first++;
            }
        }
    }

    /**
     * @brief  copy constructor
     * @param vectorToCopy
     */
    VLVector(const VLVector &vectorToCopy)
    {
        _size = vectorToCopy._size;
        _capacity = vectorToCopy._capacity;
        if (_size > capVLV)
        {
            _arr = new T[_capacity];
            for (unsigned int i = 0; i < _size; i++)
            {
                _arr[i] = vectorToCopy._arr[i];
            }
        }
        else
        {
            for (unsigned int i = 0; i < _size; i++)
            {
                arr[i] = vectorToCopy.arr[i];
            }
        }
    }

    /**
     * @brief destructor
     */
    ~VLVector()
    {
        if (_arr != NULL)
        {
            delete[] _arr;
        }
    }

    /**
     * @brief returns the size of the vector
     * @return size_type size of vec
     */
    size_t size() const
    {
        return _size;
    }

    /**
     * @brief returns the capacity
     * @return size_t
     */
    size_t capacity() const
    {
        return _capacity;
    }

    /**
     * @brief checks if the vector is empty
     * @return true if so or false.
     */
    bool empty() const
    {
        return _size == 0;
    }

    /**
     * @brief at function - returns a referance to the the item in the requested index
     * or throws error otherwise
     * @param index - the requested index
     * @return the item in this index
     */
    T &at(size_t index)
    {
        if (index >= _size)
        {
            throw std::out_of_range("index not in range");
        }
        if (_capacity == capVLV)
        {
            return arr[index];
        }
        return _arr[index];
    }

    /**
     * @brief sace as above but const
     * @param index
     * @return
     */
    const T &at(size_t index) const
    {
        if (index >= _size)
        {
            throw std::out_of_range("index not in range");
        }
        if (_capacity == capVLV)
        {
            return arr[index];
        }
        return _arr[index];
    }

    /**
     * @brief helper function for this class which is not part of the formal API - it is used to
     * extend the array if needed. and copy all elements
     * @param k  - how many elements are added - not mandatory.
     */
    void resizeToHeap(size_t k = 1)
    {
        size_t oldCap = _capacity;
        _capacity = this->capC(_size, k);
        T *newArray = new T[_capacity];
        if (oldCap == capVLV)
        {
            for (unsigned int i = 0; i < _size; i++)
            {
                newArray[i] = arr[i];
            }
        }
        else
        {
            for (unsigned int i = 0; i < _size; i++)
            {
                newArray[i] = _arr[i];
            }
            delete[](_arr);
        }
        _arr = newArray;
    }

    /**
     * @brief enters a value to the end of the vector and extends it if needed
     * @param val the value to insert.
     */
    void push_back(const T &val)
    {
        if (this->empty() && _capacity == capVLV)
        {
            arr[0] = val;
            _size++;
            return;
        }
        else if (this->empty() && _capacity > capVLV)
        {
            _arr[0] = val;
            _size++;
            return;
        }
        if (_size + 1 > _capacity)
        {
            resizeToHeap();
            _arr[_size] = val;
        }
        else if (_capacity == capVLV)
        {
            arr[_size] = val;
        }
        else
        {
            _arr[_size] = val;
        }
        _size++;
    }

    /**
     * @brief this function pops the last element in the vector, if empty behaviour is undefined
     * like the real vector
     */
    void pop_back()
    {
        if (_capacity == capVLV)
        {
        }
        else
        {
            if (_size - 1 == capVLV)
            {
                for (size_t i = 0; i < capVLV; i++)
                {
                    arr[i] = _arr[i];
                }
                delete[](_arr);
                _arr = NULL;
                _capacity = capVLV;
            }
        }
        _size = _size - 1;
    }

    /**
     * @brief clears all objects from the vector. and returns it to base status.
     */
    void clear()
    {
        if (_capacity == capVLV)
        {
            _size = 0;
        }
        else
        {
            delete[](_arr);
            _arr = NULL;
            _size = 0;
            _capacity = capVLV;
        }
    }

    /**
     * @brief returns a pointer to the current data structure used
     * @return a T pointer to the current data structure used by the VLVector class
     */
    T *data()
    {
        T *ptr;
        if (_capacity > capVLV)
        {
            ptr = _arr;
        }
        else
        {
            ptr = arr;
        }
        return ptr;
    }

    /**
     * @brief returns a pointer to the current data structure used - const ver
     * @return a T pointer to the current data structure used by the VLVector class - const ver
     */
    const T *data() const noexcept
    {
        T *ptr;
        if (_capacity > capVLV)
        {
            ptr = _arr;
        }
        else
        {
            ptr = arr;
        }
        return ptr;
    }

    /**
     * @brief returns the value in the index - if the index is greater than size than
     * undefined like vector
     * @param index - index to get element from
     * @return T element in the index
     */
    T &operator[](size_t index)
    {
        return this->data()[index];
    }

    /**
    * @brief returns the value in the index - if the index is greater than size than
    * undefined like vector - const version
    * @param index - index to get element from
    * @return T element in the index
    */
    const T &operator[](size_t index) const
    {
        return this->data()[index];
    }

    /**
     * @brief keep notice - if the capacity is different but size the same and all elements the same
     * order they are considered equal
     * @param toCompareVector the second vector
     * @return true if both vectors are identical or false otherwise
     */
    bool operator==(const VLVector &toCompareVector) const
    {
        bool flag = true;
        if (_size == toCompareVector._size)
        {
            for (size_t i = 0; i < _size; i++)
            {
                if (!(this->at(i) == toCompareVector.at(i)))
                {
                    flag = false;
                    break;
                }
            }
        }
        else
        {
            flag = false;
        }
        return flag;
    }

    /**
     * @brief the opposite of operator ==
     * @param toCompareVector the vector to check if not equal
     * @return the opposite of operator ==
     */
    bool operator!=(const VLVector &toCompareVector) const
    {
        return !(*this == toCompareVector);
    }

    /**
     * @brief copies elements from one vector to another!, does not change the STATIC cap of the
     * vector we put items into, but does make sure if the cap is beyond of this vectors static
     * cap we increase it to match the other vector capacity exactly
     * @param vector the vector we copy from
     * @return an exact copy! (item wise - cap may differ according to static the vector was
     * created with)
     */
    VLVector &operator=(const VLVector &vector)
    {
        if (this == &vector)
        {
            return *this;
        }
        this->clear();
        _size = vector._size;
        _capacity = vector._capacity;
        if (vector._capacity > capVLV)
        {
            _arr = new T[_capacity];
            if (vector._arr != NULL)
            {
                for (size_t i = 0; i < _size; i++)
                {
                    _arr[i] = vector._arr[i];
                }
            }
            else
            {
                for (size_t i = 0; i < _size; i++)
                {
                    _arr[i] = vector.arr[i];
                }
            }
        }
        else
        {
            if (vector._arr != NULL)
            {
                for (size_t i = 0; i < _size; i++)
                {
                    arr[i] = vector._arr[i];
                }
            }
            else
            {
                for (size_t i = 0; i < _size; i++)
                {
                    arr[i] = vector.arr[i];
                }
            }
        }
        return *this;
    }

    // i did not provide description these next methods - self explanatory!
    const_iterator begin() const noexcept
    {
        return this->data();
    }

    const_iterator end() const noexcept
    {
        return this->data() + _size;
    }

    iterator begin() noexcept
    {
        return this->data();
    }

    iterator end() noexcept
    {
        return this->data() + _size;
    }

    const_iterator cbegin() noexcept
    {
        return this->data();
    }

    const_iterator cend() noexcept
    {
        return this->data() + _size;
    }

    /**
     * @brief this fucntion is called when we move from one array to another in memory so the mem
     * isnt linear anymore so we need to change the iterator to point to the proper value again!
     * @param distance
     * @return
     */
    iterator catchUp(size_t distance)
    {
        iterator it = this->begin();
        return it + distance;
    }

    /**
     * @brief we insert the value in the specified spot - checking first we have enough space
     * and than moving each item 1 place to the "right" in out array and make sure we are pointing
     * to the proper array in memory
     * @param inPlace the iterator to the first item to move
     * @param val the value to be entered before
     * @return iterator to the newly inserted item!
     */
    iterator insert(const_iterator inPlace, const T &val)
    {
        iterator inPlaceNonC = this->begin();
        size_t dis = inPlace - inPlaceNonC;
        T predValue = *inPlace;
        inPlaceNonC = inPlaceNonC + dis;
        if (_size + 1 > _capacity)
        {
            resizeToHeap();
            inPlaceNonC = catchUp(dis);
        }
        *inPlaceNonC = val;
        for (iterator iterTemp = inPlaceNonC + 1; iterTemp != this->end(); iterTemp++)
        {
            T currentMove = *iterTemp;
            *iterTemp = predValue;
            predValue = currentMove;
        }
        this->push_back(predValue);
        return inPlaceNonC;
    }

    /**
     * @brief same as insert but instead we insert a range of items in the designated place!
     * @tparam InputIterator template for the iterator type for our input
     * @param inPlace the element we need to add the range before
     * @param first the iterator to the first element
     * @param last the iterator to the last element
     * @return iterator to the newly inserted item.
     */
    template<class InputIterator>
    iterator insert(const_iterator inPlace, InputIterator first, InputIterator last)
    {
        InputIterator firstCopy = first;
        size_t k = 0;
        while (firstCopy != last)
        {
            firstCopy++;
            k++;
        }
        iterator inPlaceNonC = this->begin();
        size_t dis = inPlace - inPlaceNonC;
        inPlaceNonC = inPlaceNonC + dis;
        if (_size + k > _capacity || (_size + k <= _capacity && _capacity > capVLV))
        {
            size_t cap = capC(_size, k);
            T *extended = new T[cap];
            int i = 0;
            for (iterator iterator1 = this->begin(); iterator1 != inPlaceNonC; iterator1++)
            {
                extended[i] = *iterator1;
                i++;
            }
            for (InputIterator iterator1 = first; iterator1 != last; iterator1++)
            {
                extended[i] = *iterator1;
                i++;
            }
            for (iterator iterator1 = inPlaceNonC; iterator1 != this->end(); iterator1++)
            {
                extended[i] = *iterator1;
                i++;
            }
            delete[](_arr);
            _capacity = cap;
            _arr = extended;
            inPlaceNonC = catchUp(dis);
        }
        else
        {
            T array[capVLV * sizeof(T)];
            int i = 0;
            for (iterator iterator1 = this->begin(); iterator1 != inPlaceNonC; iterator1++)
            {
                array[i] = *iterator1;
                i++;
            }
            for (InputIterator iterator1 = first; iterator1 != last; iterator1++)
            {
                array[i] = *iterator1;
                i++;
            }
            for (iterator iterator1 = inPlaceNonC; iterator1 != this->end(); iterator1++)
            {
                array[i] = *iterator1;
                i++;
            }
            for (size_t j = 0; j < _size + k; j++)
            {
                arr[j] = array[j];
            }
        }
        _size = _size + k;
        return inPlaceNonC;
    }

    /**
     * @brief gets an iterator to an element to remove and removes it.
     * @param toRemove - position of element - const iterator
     * @return iterator to the element that was to the right. can be end().
     */
    iterator erase(const_iterator toRemove)
    {
        iterator toReturn = this->begin();
        size_t index = toRemove - toReturn;
        toReturn = catchUp(index);
        iterator toReturn1 = toReturn;
        T valueRemove = *toRemove;
        for (iterator iterTemp = toReturn + 1; iterTemp != this->end(); iterTemp++)
        {
            T currentMove = *iterTemp;
            *toReturn1 = currentMove;
            toReturn1++;
        }
        *toReturn1 = valueRemove;
        pop_back();
        toReturn = catchUp(index);
        return toReturn;
    }

    /**
     * @brief as the above erase function but it removes from a range of first - last
     * @param first the first pos to remove
     * @param last the last position - not to be removed
     * @return the element that was to right - iterator to it - could be end()
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        size_t k = last - first;
        size_t diffBegin = first - this->begin();
        iterator toReturn;
        iterator lastNoC = catchUp(diffBegin + k);
        if (_size - k <= capVLV)
        {
            int i = 0;
            T array[capVLV * sizeof(T)];
            for (iterator iterator1 = this->begin(); iterator1 != first; iterator1++)
            {
                array[i] = *iterator1;
                i++;
            }
            for (iterator iterator1 = lastNoC; iterator1 != this->end(); iterator1++)
            {
                array[i] = *iterator1;
                i++;
            }
            for (size_t j = 0; j < _size - k; j++)
            {
                arr[j] = array[j];
            }
            delete[](_arr);
            _arr = NULL;
            _capacity = capVLV;
            _size = _size - k;
            toReturn = catchUp(diffBegin);
        }
        else
        {
            T *tempArray = new T[_capacity];
            int i = 0;
            for (iterator iterator1 = this->begin(); iterator1 != first; iterator1++)
            {
                tempArray[i] = *iterator1;
                i++;
            }
            for (iterator iterator1 = lastNoC; iterator1 != this->end(); iterator1++)
            {
                tempArray[i] = *iterator1;
                i++;
            }
            delete[](_arr);
            _arr = tempArray;
            size_t diff = _size - k;
            while (_size != diff)
            {
                pop_back();
            }
            toReturn = catchUp(diffBegin);
        }
        return toReturn;
    }
};

#endif //VLVECTOR_VLVECTOR_HPP
