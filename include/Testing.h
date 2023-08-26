#pragma once

#include "Pair.h"
#include "Tuple.h"
#include "String.h"
#include "Array.h"
#include "Vector.h"
#include "Deque.h"
#include "List.h"
#include "Queue.h"
#include "Stack.h"
#include "UnorderedMap.h"
#include "UnorderedSet.h"
#include "Map.h"
#include "Set.h"
#include "Functional.h"
#include "Memory.h"
#include "Limits.h"
#include "Chrono.h"

#if defined __GNUG__
#include "Thread.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "Barrier.h"
#endif	// __GNUG__

#include <string>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <functional>
#include <memory>
#include <limits>
#include <chrono>

#include <thread>
#include <mutex>
#include <semaphore>
#include <barrier>

#define TEST_BEGIN namespace test {
#define TEST_END }


TEST_BEGIN

class Test {

public:
	int value = 0;

	Test() {
		std::cout << "Default Construct\n";
	}

	Test(int x) {
		value = x;
		std::cout << "Custom Construct " << value << '\n';
	}

	Test(const Test& other) {
		value = other.value;
		std::cout << "Copy Construct " << value << '\n';
	}

	Test(Test&& other) noexcept {
		value = custom::move(other.value);
		std::cout << "Move Construct " << value << '\n';
	}

	~Test() {
		std::cout << "Destruct " << value << '\n';
	}

	Test& operator=(const Test& other) {
		value = other.value;
		std::cout << "Copy Assign " << value << '\n';
		return *this;
	}

	Test& operator=(Test&& other) noexcept {
		value = custom::move(other.value);
		std::cout << "Move Assign " << value << '\n';
		return *this;
	}

	bool operator==(const Test& other) const {
		return value == other.value;
	}

	bool operator!=(const Test& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Test& test) {
		os << "Test val = " << test.value;
		return os;
	}

	int test_function(int x) {
		return x;
	}
};

template<typename T, T... ints>
void print_sequence(custom::IntegerSequence<T, ints...> int_seq)
{
	std::cout << "The sequence of size " << int_seq.size() << ": ";
	((std::cout << ints << ' '), ...);
	std::cout << '\n';
}

void print_sequence_test() {
	print_sequence(custom::IntegerSequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
	print_sequence(custom::MakeIntegerSequence<int, 20>{});
	print_sequence(custom::MakeIndexSequence<10>{});
	print_sequence(custom::IndexSequenceFor<float, std::iostream, char>{});
}

void piecewise_pair_test() {
	 custom::Pair<int, Test> p(
		 custom::PiecewiseConstruct,
		 custom::forward_as_tuple(3),
		 custom::forward_as_tuple(3));

	std::cout << p.First << ' ' << p.Second << '\n';
}

void tuple_test() {
	{
		std::tuple<int, Test> tuple1(3, Test(3));
		std::tuple<Test, int, float> tuple2(Test(3), 3, 3.3);
		auto tuple3 = std::tuple_cat(std::move(tuple1), std::move(tuple2));
		std::cout << std::get<4>(tuple3) << '\n';
		std::cout << std::get<1>(tuple1) << '\n';
	}

	std::cout << "\n\n";

	{
		custom::Tuple<int, Test> tuple1(3, Test(3));
		custom::Tuple<Test, int, float> tuple2(Test(3), 3, 3.3);
		auto tuple3 = custom::tuple_cat(custom::move(tuple1), custom::move(tuple2));
		std::cout << custom::get<4>(tuple3) << '\n';
		std::cout << custom::get<1>(tuple1) << '\n';
	}
}

void list_test() {
	custom::List<Test> list;
	custom::List<Test> list1;

	list.emplace_back(1);
	list.emplace_back(2);
	list.emplace_back(3);
	list.emplace_front(4);

	list1 = custom::move(list);
	list = custom::move(list1);

	for (auto it = list.begin(); it != list.end(); ++it)
		std::cout << *it << '\n';
}

void unordered_map_test() {
	custom::UnorderedMap<int, custom::String> map;

	map[10] = "abc";
	map[11] = "gsa";
	map[26] = "af";
	map[101] = " a0";
	map[54] = "69";
	map[1] = "A bv";
	map.emplace(3, "abc");
	map.try_emplace(56, custom::String("shmecher"));

	std::cout << map.at(10) << '\n';

	map.print_details();
	std::cout << '\n';
	for (auto& val : map)
		std::cout << val.First << ' ' << val.Second << '\n';
}

void unordered_set_test() {
	custom::UnorderedSet<custom::String> set;
	custom::UnorderedSet<custom::String> set1;

	set.emplace("abc");
	set.emplace("def");
	set.emplace("ghi");
	set.emplace("jkl");
	set1 = custom::move(set);
	set = custom::move(set1);

	set.print_details();
}

void map_test() {
	custom::Map<int, int> map;

	map.emplace(5, 5);
	map.emplace(0, 0);
	map.emplace(7, 7);
	map.emplace(1, 1);
	map.emplace(10, 10);
	map.emplace(21, 21);
	map.emplace(44, 44);
	map.emplace(6, 6);
	map.emplace(8, 8);
	map.emplace(15, 15);
	map.emplace(14, 14);
	map.emplace(96, 96);
	map.emplace(2, 2);
	map.emplace(50, 50);
	map.emplace(69, 69);
	map.emplace(19, 19);
	map.emplace(29, 29);
	map.emplace(101, 101);
	map.emplace(40, 40);
	map.emplace(51, 51);
	map.emplace(99, 99);
	map.emplace(99, 99);

	std::cout << "Map before erase\n";
	map.print_details();

	std::cout << "Current elem= " << (*map.erase(10)).First << '\n';

	std::cout << "Map after erase\n";
	map.print_details();

	for (auto& val : map)
		std::cout << val.First << ' ' << val.Second << '\n';
}

void deque_test() {
	custom::Deque<int> dq;
	dq.emplace_back(0);
	dq.emplace_back(1);
	dq.emplace_back(2);
	dq.emplace_back(3);
	dq.emplace_front(4);
	auto it = dq.emplace(dq.end() - 1, 11);
	it = dq.emplace(it, 12);

	it = dq.pop(dq.begin());
	it = dq.pop(it);

	for (auto& val : dq)
		std::cout << val << ' ';
	std::cout << '\n';

	dq.print_details();
}

void queue_test() {
	custom::Queue<Test> q;

	for (size_t i = 0; i < 50; ++i)
		q.emplace(i);

	while (!q.empty())
	{
		std::cout << q.front() << '\n';
		q.pop();
	}
}

void priority_queue_test() {
	custom::PriorityQueue<int> pq;

	pq.emplace(0);
	pq.emplace(5);
	pq.emplace(1);
	pq.emplace(45);
	pq.emplace(101);
	pq.emplace(56);
	pq.emplace(12);
	pq.emplace(7);
	pq.emplace(11);

	pq.print_details();

	while (!pq.empty())
	{
		std::cout << pq.front() << '\n';
		pq.pop();
	}
}

void stack_test() {
	custom::Stack<int> s;

	s.push(0);
	s.push(3);
	s.push(1);
	s.push(2);

	while (!s.empty())
	{
		std::cout << s.top() << '\n';
		s.pop();
	}
}

void vector_test() {
	custom::Vector<int> v;
	v.emplace_back(0);
	v.emplace_back(1);
	v.emplace_back(2);

	custom::Vector<int> v1(v);


	for (auto& val : v1)
		std::cout << val << '\n';

	std::cout << v1.size() << ' ' << v1.capacity() << '\n';
}

void string_test() {
	custom::String s("abcd");
	custom::String s1("abcd");
	s.insert(++s.begin(),s1.begin(),--s1.end());

	std::string ss("abcd");
	std::string ss1("abcd");
	ss.insert(++ss.begin(),ss1.begin(),--ss1.end());
	std::cout << ss << '\n';

	s.print_details();
}

void array_test() {
	custom::Array<Test, 5> arr;
	arr.fill(Test(3));

	for (auto& val : arr)
		std::cout << val << '\n';
}

void function_test() {
	// custom::Function<void()> fct(unordered_map_test);
	// custom::Function<void()> fct1(fct);
	// fct1();
}

void invoke_test() {
	Test t;
	std::cout << custom::invoke(&Test::test_function, t, 3) << '\n';
}

#if defined __GNUG__
void thread_test() {
	std::cout << custom::this_thread::get_id();
	// custom::Thread t(deque_test);
	// t.join();
}
#endif	// __GNUG__ for thread tests

void memory_test() {
	custom::UniquePtr<Test> up = custom::make_unique<Test>(3);
}

TEST_END



STD_BEGIN
template<>
struct less<test::Test> {
	bool operator()(const test::Test& left, const test::Test& right) const {
		return left.value < right.value;
	}
};
STD_END