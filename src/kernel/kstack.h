#ifndef KERNEL_STACK_H
#define KERNEL_STACK_H

#include <deque>

template<typename T>
class kstack
	:private std::deque<T>
{
public:
	kstack() {}
	bool empty() const { return d.empty(); }
	T pop() 
	{ 
		T ret = d.back();
		d.pop_back();
		return ret; 
	} 
	void push(const T& v)
	{
		d.push_back(v);
	}
	void pop_front() { d.pop_front(); }
	T front() { return d.front(); }
	void clear() { d.clear(); }
private:
	std::deque<T> d;
};

#endif
