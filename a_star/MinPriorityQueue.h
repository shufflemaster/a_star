#pragma once
#include <functional>
#include <vector>
#include <algorithm>

/*
The default implementation in STL library doesn't support resorting
the queue.
*/
template <class T, class Container = std::vector<T>,
	class Compare = std::greater<T> >
class MinPriorityQueue
{
protected:
	Container c;
	Compare comp;
public:
	explicit MinPriorityQueue(const Container& c_ = Container(),
		const Compare& comp_ = Compare())
		: c(c_), comp(comp_)
	{
		std::make_heap(c.begin(), c.end(), comp);
	}
	bool empty()       const { return c.empty(); }
	std::size_t size() const { return c.size(); }
	const T& top()     const { return c.front(); }
	void push(const T& x)
	{
		c.push_back(x);
		std::push_heap(c.begin(), c.end(), comp);
	}
	void pop()
	{
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}
	void resort()
	{
		std::make_heap(c.begin(), c.end(), comp);
	}
};

