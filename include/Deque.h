#pragma once
#include "Allocator.h"
#include "Utility.h"


CUSTOM_BEGIN

template<class Type>
struct DequeData
{
	using ValueType		= Type;						// Type for stored values
	using IterType		= ValueType;				// Type for iteration (same as value)
	using Alloc			= Allocator<ValueType>;		// Allocator type

	ValueType* _Base	= nullptr;					// Actual container array
	ValueType* _Front	= nullptr;					// Pointer to first elem
	ValueType* _End		= nullptr;					// Pointer to end ( back == _End - 1 )
	ValueType* _Final	= nullptr;					// Pointer to capacity end

	void increment(ValueType* ptr) const {
		if (ptr == _Final + 1)
			ptr = _Base;
		else
			++ptr;
	}

	void increment(ValueType* ptr, const size_t& count) const {
		size_t index 	= static_cast<size_t>(ptr - _Base);
		size_t capacity = static_cast<size_t>(_Final - _Base);
		ptr 			= _Base + (index + count) % (capacity + 1);
	}

	void decrement(ValueType* ptr) const {
		if (ptr == _Base)
			ptr = _Final + 1;
		else
			--ptr;
	}

	void decrement(ValueType* ptr, const size_t& count) const {
		size_t index 	= static_cast<size_t>(ptr - _Base);
		size_t capacity = static_cast<size_t>(_Final - _Base);
		ptr 			= _Base + ((capacity + 1) + index - count) % (capacity + 1);
	}
};


template<class Deque>
class DequeConstIterator
{
public:
	using Data			= typename Deque::Data;
	using ValueType		= typename Deque::ValueType;
	using IterType		= typename Deque::IterType;

	IterType* _Ptr		= nullptr;
	const Data* _Data	= nullptr;

public:

	explicit DequeConstIterator(IterType* ptr, const Data* data)
		:_Ptr(ptr), _Data(data) { /*Empty*/ }

	DequeConstIterator& operator++() {
		if (_Ptr == _Data->_End)
			throw std::out_of_range("Cannot increment end iterator...");

		_Data->increment(_Ptr);
		//_Ptr = _Data->_Base + Deque::circular_increment(get_index(), _Data->_Capacity);
		return *this;
	}

	DequeConstIterator operator++(int) {
		DequeConstIterator temp = *this;
		++(*this);
		return temp;
	}

	DequeConstIterator& operator+=(const size_t& diff) {
		if(get_pos() + diff > _Data->_Size)
			throw std::out_of_range("Cannot increment end iterator...");

		_Data->increment(_Ptr, diff);
		//_Ptr = _Data->_Base + Deque::circular_increment(get_index(), _Data->_Capacity, diff);
		return *this;
	}

	DequeConstIterator operator+(const size_t& diff) const {
		DequeConstIterator temp = *this;
		temp += diff;
		return temp;
	}

	DequeConstIterator& operator--() {
		if (_Ptr == _Data->_Begin)
			throw std::out_of_range("Cannot decrement begin iterator...");

		_Data->decrement(_Ptr);
		//_Ptr = _Data->_Base + Deque::circular_decrement(get_index(), _Data->_Capacity);
		return *this;
	}

	DequeConstIterator operator--(int) {
		DequeConstIterator temp = *this;
		--(*this);
		return temp;
	}

	DequeConstIterator& operator-=(const size_t& diff) {
		if(get_pos() < diff)
			throw std::out_of_range("Cannot decrement begin iterator...");

		_Data->decrement(_Ptr, diff);
		//_Ptr = _Data->_Base + Deque::circular_decrement(get_index(), _Data->_Capacity, diff);
		return *this;
	}

	DequeConstIterator operator-(const size_t& diff) const {
		DequeConstIterator temp = *this;
		temp -= diff;
		return temp;
	}

	const IterType* operator->() const {
		if (_Ptr == _Data->_End)
			throw std::out_of_range("Cannot access end iterator...");

		return _Ptr;
	}

	const ValueType& operator*() const {
		if (_Ptr == _Data->_End)
			throw std::out_of_range("Cannot dereference end iterator...");

		return *_Ptr;
	}

	bool operator==(const DequeConstIterator& other) const {
		return _Ptr == other._Ptr;
	}

	bool operator!=(const DequeConstIterator& other) const {
		return !(*this == other);
	}

public:

	const size_t get_index() const {						// Get the index for the element in array from iterator (relative to array address, NOT begin)
		return static_cast<size_t>(_Ptr - _Data->_Base);
	}

	const size_t get_pos() const {							// Get the position for the element in array from iterator (relative to begin)
		// TODO: remake
		size_t frontIndex = static_cast<size_t>(_Data->_Front - _Data->_Base);
		return Deque::circular_decrement(get_index(), _Data->_Capacity, frontIndex);	// position relative to front
	}

	const bool is_begin() const {
		return _Ptr == _Data->_Front;
	}

	const bool is_end() const {
		return _Ptr == _Data->_End;
	}
};

template<class Deque>
class DequeIterator : public DequeConstIterator<Deque>			// Deque Iterator
{
private:
	using Base		= DequeConstIterator<Deque>;

public:
	using Data		= typename Deque::Data;
	using ValueType	= typename Deque::ValueType;
	using IterType	= typename Deque::IterType;

public:

	explicit DequeIterator(IterType* ptr, Data* data)
		:Base(ptr, data) { /*Empty*/ }

	DequeIterator& operator++() {
		Base::operator++();
		return *this;
	}

	DequeIterator operator++(int) {
		DequeIterator temp = *this;
		Base::operator++();
		return temp;
	}

	DequeIterator& operator+=(const size_t& diff) {
		Base::operator+=(diff);
		return *this;
	}

	DequeIterator operator+(const size_t& diff) const {
		DequeIterator temp = *this;
		temp += diff;
		return temp;
	}

	DequeIterator& operator--() {
		Base::operator--();
		return *this;
	}

	DequeIterator operator--(int) {
		DequeIterator temp = *this;
		Base::operator--();
		return temp;
	}

	DequeIterator& operator-=(const size_t& diff) {
		Base::operator-=(diff);
		return *this;
	}

	DequeIterator operator-(const size_t& diff) const {
		DequeIterator temp = *this;
		temp -= diff;
		return temp;
	}

	IterType* operator->() const {
		return const_cast<IterType*>(Base::operator->());
	}

	ValueType& operator*() const {
		return const_cast<ValueType&>(Base::operator*());
	}
}; // END Deque Iterator


template<class Type>
class Deque					// Deque Template implemented as circular array
{
public:
	using Data					= DequeData<Type>;							// Members that are modified
	using ValueType 			= typename Data::ValueType;					// Type for stored values
	using IterType				= typename Data::IterType;					// Type for iteration (same as value)
	using Alloc					= typename Data::Alloc;						// Allocator type
	
	using Iterator				= DequeIterator<Deque<ValueType>>;			// Iterator type
	using ConstIterator			= DequeConstIterator<Deque<ValueType>>;		// Const Iterator type
	using ReverseIterator 		= custom::ReverseIterator<Iterator>;		// ReverseIterator type
	using ConstReverseIterator	= custom::ReverseIterator<ConstIterator>;	// Const Reverse Iterator type

private:
	Data _data;
	Alloc _alloc;											// Allocator
	
	static constexpr size_t default_capacity = 8;

public:
	// Constructors

	Deque() {																	// Default Constructor
		reserve(default_capacity);
	}

	Deque(const size_t& newCapacity, const ValueType& copyValue) {				// Add multiple copies Constructor
		realloc(newCapacity, copyValue);
	}

	Deque(const Deque& other) {													// Copy Constructor
		_copy(other);
	}

	Deque(Deque&& other) noexcept {												// Move Constructor
		_move(custom::move(other));
	}

	~Deque() {
		_clean_up_array();
	}

public:
	// Main functions

	static size_t circular_increment(const size_t& index, const size_t& capacity, const size_t& count = 1) {	// increment and return index
		return (index + count) % (capacity + 1);
	}

	static size_t circular_decrement(const size_t& index, const size_t& capacity, const size_t& count = 1) {	// decrement and return index
		return ((capacity + 1) + index - count) % (capacity + 1);
	}

	void reserve(const size_t& newCapacity) {									// Allocate memory and move values if needed
		if (newCapacity < size())
			_data._End = _data._Base + newCapacity;

		ValueType* newArray = _alloc_array(newCapacity);
		ValueType* front 	= _data._Front;
		size_t newSize 		= size();
		for (size_t i = 0; i < newSize; ++i)
		{
			_alloc.construct(&newArray[i], custom::move(front[0]));
			_data.increment(front);
		}

		_clean_up_array();
		_data._Base 	= newArray;
		_data._Front 	= _data._Base;
		_data._End 		= _data._Base + newSize;
		_data._Final 	= _data._Base + newCapacity;
	}

	void shrink_to_fit() {														// Allocate memory with capacity equal to size and move values there
		reserve(size());
	}

	void realloc(const size_t& newCapacity) {									// Allocate memory and populate it with default values (delete old)
		_clean_up_array();

		_data._Base 	= _alloc_array(newCapacity);
		_data._Front 	= _data._Base;
		_data._End 		= _data._Base + newCapacity;
		_data._Final 	= _data._Base + newCapacity;
		_alloc.construct_range(_data._Base, newCapacity);
	}

	void realloc(const size_t& newCapacity, const ValueType& copyValue) {		// Allocate memory and populate it with given reference (delete old)
		_clean_up_array();

		_data._Base 	= _alloc_array(newCapacity);
		_data._Front 	= _data._Base;
		_data._End 		= _data._Base + newCapacity;
		_data._Final 	= _data._Base + newCapacity;
		_alloc.construct_range(_data._Base, newCapacity, copyValue);
	}

	void resize(const size_t& newSize) {										// Change size and Construct/Destruct objects with default value if needed
		// TODO: implement
	}

	void resize(const size_t& newSize, const ValueType& copyValue) {			// Change size and Construct/Destruct objects with given reference if needed
		// TODO: implement
	}

	template<class... Args>
	void emplace_back(Args&&... args) {											// Construct object using arguments (Args) and add it to the tail
		_extend_if_full();
		_alloc.construct(_data._End, custom::forward<Args>(args)...);
		_data.increment(_data._End);
	}

	void push_back(const ValueType& copyValue) {								// Construct object using reference and add it to the tail
		emplace_back(copyValue);
	}

	void push_back(ValueType&& moveValue) {										// Construct object using temporary and add it to the tail
		emplace_back(custom::move(moveValue));
	}

	void pop_back() {															// Remove last component
		if (!empty())
		{
			_data.decrement(_data._End);
			_alloc.destroy(_data._End);
		}
	}

	template<class... Args>
	void emplace_front(Args&&... args) {										// Construct object using arguments (Args) and add it to the front
		_extend_if_full();
		_data.decrement(_data._Front);
		_alloc.construct(_data._Front, custom::forward<Args>(args)...);
	}

	void push_front(const ValueType& copyValue) {								// Construct object using reference and add it to the front
		emplace_front(copyValue);
	}

	void push_front(ValueType&& moveValue) {									// Construct object using temporary and add it to the front
		emplace_front(custom::move(moveValue));
	}

	void pop_front() {															// Remove first component
		if (!empty())
		{
			_alloc.destroy(_data._Front);
			_data.increment(_data._Front);
		}
	}

	// template<class... Args>
	// Iterator emplace(const Iterator& iterator, Args&&... args) {				// Emplace object at iterator position with given arguments
	// 	size_t pos = iterator.get_pos();							// position relative to front
	// 	emplace_back();
	// 	size_t index = circular_increment(_front, _capacity, pos);	// real index after (possible) realocation

	// 	for(size_t i = _back; i != index; i = circular_decrement(i, _capacity))
	// 		_array[i] = custom::move(_array[circular_decrement(i, _capacity)]);

	// 	_alloc.destroy(&_array[index]);
	// 	_alloc.construct(&_array[index], custom::forward<Args>(args)...);

	// 	return Iterator(&_array[index], _update_iteration_data());
	// }

	// Iterator push(const Iterator& iterator, const ValueType& copyValue) {		// Push copy object at iterator position
	// 	return emplace(iterator, copyValue);
	// }

	// Iterator push(const Iterator& iterator, ValueType&& moveValue) {			// Push temporary object at iterator position
	// 	return emplace(iterator, custom::move(moveValue));
	// }

	// Iterator pop(const Iterator& iterator) {									// Remove component at iterator position
	// 	if (iterator.is_end())
	// 		throw std::out_of_range("Array pop iterator outside range...");

	// 	size_t index = iterator.get_index();
	// 	for (size_t i = index; i != circular_increment(_back, _capacity) && _size > 0; i = circular_increment(i, _capacity))
	// 		_array[i] = custom::move(_array[circular_increment(i, _capacity)]);
	// 	pop_back();

	// 	return Iterator(&_array[index], _update_iteration_data());
	// }

	ValueType& front() {                                                     	// Get the value of the first component
		assert(!empty());
		return _data._Front[0];
	}

	const ValueType& front() const {
		assert(!empty());
		return _data._Front[0];
	}

	ValueType& back() {                                                      	// Get the value of the last component
		assert(!empty());
		ValueType* ptr = _data._End;
		_data.decrement(ptr);
		return ptr[0];
	}

	const ValueType& back() const {
		assert(!empty());
		ValueType* ptr = _data._End;
		_data.decrement(ptr);
		return ptr[0];
	}

	const size_t capacity() const {												// Get capacity
		return static_cast<size_t>(_data._Final - _data._Base - 1);
	}

	const size_t size() const {													// Get size
		if (_data._Front <= _data._End)
			return static_cast<size_t>(_data._End - _data._Front);
		else
			return static_cast<size_t>(_data._Final - _data._Front + _data._End - _data._Base);
	}

	void clear() {																// Remove ALL components but keep memory
		_destroy_array();
		_data._End 		= _data._Base;
		_data._Front 	= _data._Base;
	}

	bool empty() const {														// Check if array is empty
		return _data._Front == _data._End;
	}

	const ValueType& at(const size_t& index) const {							// Acces object at index with check (read only)
		if (index >= size())
			throw std::out_of_range("Invalid Index...");

		ValueType* ptr = _data._Front;
		_data.increment(ptr, index);
		return ptr[0];
	}

	ValueType& at(const size_t& index) {										// Acces object at index with check
		if (index >= size())
			throw std::out_of_range("Invalid Index...");

		ValueType* ptr = _data._Front;
		_data.increment(ptr, index);
		return ptr[0];
	}

	void print_details() {
		std::cout << "Size= " << size() << '\n';
		std::cout << "Capacity= " << capacity() << '\n';
		// std::cout << "Front= " << front() << '\n';
		// std::cout << "Back= " << back() << '\n';

		for (size_t i = 0; i <= capacity(); ++i)
			std::cout << _data._Base[i] << ' ';

		std::cout << "\n\n";
	}

public:
	// Operators

	const ValueType& operator[](const size_t& index) const {					// Acces object at index (read only)
		assert(!(index >= size()));
		ValueType* ptr = _data._Front;
		_data.increment(ptr, index);
		return ptr[0];
	}

	ValueType& operator[](const size_t& index) {								// Acces object at index
		assert(!(index >= size()));
		ValueType* ptr = _data._Front;
		_data.increment(ptr, index);
		return ptr[0];
	}

	Deque& operator=(const Deque& other) {										// Assign operator using reference
		if (_data._Base != other._data._Base)
		{
			_clean_up_array();
			_copy(other);
		}

		return *this;
	}

	Deque& operator=(Deque&& other) noexcept {									// Assign operator using temporary
		if (_data._Base != other._data._Base)
		{
			_clean_up_array();
			_move(custom::move(other));
		}

		return *this;
	}

	bool operator==(const Deque& other) const {
		if (size() != other.size())
			return false;

		auto it1 = begin();
		auto it2 = other.begin();
		while (it1 != end())
		{
			if (*it1 != *it2)
				return false;

			++it1;
			++it2;
		}

		return true;
	}

	bool operator!=(const Deque& other) const {
		return !(*this == other);
	}

public:
	// Iterator specific functions

	Iterator begin() {
		return Iterator(_data._Front, &_data);
	}

	ConstIterator begin() const {
		return ConstIterator(_data._Front, &_data);
	}

	ReverseIterator rbegin() {
		return ReverseIterator(end());
	}

	ConstReverseIterator rbegin() const {
		return ConstReverseIterator(end());
	}

	Iterator end() {
		return Iterator(_data._End, &_data);
	}

	ConstIterator end() const {
		return ConstIterator(_data._End, &_data);
	}

	ReverseIterator rend() {
		return ReverseIterator(begin());
	}

	ConstReverseIterator rend() const {
		return ConstReverseIterator(begin());
	}

private:
	// Helpers

	ValueType* _alloc_array(const size_t& capacity) {							// Allocate memory +1 null term
		return _alloc.alloc(capacity + 1);
	}

	void _dealloc_array() {														// Deallocate memory +1 null term
		_alloc.dealloc(_data._Base, capacity() + 1);
	}

	void _destroy_array() {
		for (ValueType* front = _data._Front; front != _data._End; _data.increment(front))
			_alloc.destroy(front);
	}

	void _copy(const Deque& other) {											// Generic copy function for deque
		// _array = _alloc_array(other._capacity);

		// if (!other.empty())
		// {
		// 	size_t otherEnd = circular_increment(other._back, other._capacity);
		// 	for (size_t i = other._front; i != otherEnd; i = circular_increment(i, other._capacity))
		// 		_alloc.construct(&_array[i], other._array[i]);
		// }
		
		// _size 		= other._size;
		// _capacity 	= other._capacity;
		// _front 		= other._front;
		// _back 		= other._back;
	}

	void _move(Deque&& other) {													// Generic move function for deque
		// _array 		= other._array;
		// _size 		= other._size;
		// _capacity 	= other._capacity;
		// _front 		= other._front;
		// _back 		= other._back;

		// other._array 	= nullptr;
		// other._size 	= 0;
		// other._capacity = 0;
		// other._front 	= 0;
		// other._back 	= 0;
	}

	void _extend_if_full() {													// Reserve 50% more capacity when full
		if (size() >= capacity())
			reserve(capacity() + capacity() / 2 + 1);
	}

	void _clean_up_array() {
		if (_data._Base != nullptr)
		{
			_destroy_array();
			_dealloc_array();

			_data._Base		= nullptr;
			_data._Front 	= nullptr;
			_data._End		= nullptr;
			_data._Final	= nullptr;
		}
	}

	// Data* _update_iteration_data() const {										// Update the data used in Iterator
	// 	_data._Begin 	= _array + _front;
	// 	_data._End 		= _array + circular_increment(_front, _capacity, _size);
	// 	_data._Base 	= _array;
	// 	_data._Size 	= _size;
	// 	_data._Capacity = _capacity;

	// 	return &_data;
	// }
}; // END Deque Template

CUSTOM_END