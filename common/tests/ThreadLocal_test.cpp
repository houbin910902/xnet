#include "ThreadLocal.h"
#include "CurrentThread.h"
#include "Thread.h"

#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test: boost::noncopyable
{
public:
	Test()
	{
		printf("tid=%d, constructing %p\n", xnet::CurrentThread::tid(), this);
	}

	~Test()
	{
		printf("tid=%d, destructing %p %s\n", xnet::CurrentThread::tid(), this, name_.c_str());
	}

	const std::string& name() const
	{
		return name_;
	}
	void setName(const std::string& n)
	{
		name_ = n;
	}

private:
	std::string name_;
};

xnet::ThreadLocal<Test> testObj1;
xnet::ThreadLocal<Test> testObj2;

void print()
{
	printf("tid=%d, obj1 %p name=%s\n", xnet::CurrentThread::tid(), &testObj1.value(), testObj1.value().name().c_str());
	printf("tid=%d, obj2 %p name=%s\n", xnet::CurrentThread::tid(), &testObj2.value(), testObj2.value().name().c_str());
}

void threadFunc()
{
	print();
	testObj1.value().setName("changed 1");
	testObj2.value().setName("changed 42");
	print();
}

int main()
{
	testObj1.value().setName("main one");
	print();
	xnet::Thread t1(threadFunc);
	t1.start();
	t1.join();
	testObj2.value().setName("main two");
	print();

	pthread_exit(0);
}
