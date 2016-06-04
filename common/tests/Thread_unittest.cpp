#include "Thread.h"
#include "CurrentThread.h"

#include <string>
#include <boost/bind.hpp>
#include <stdio.h>

void threadFunc()
{
	printf("tid=%d\n", xnet::CurrentThread::tid());
}

void threadFunc2(int x)
{
	printf("tid=%d, x=%d\n", xnet::CurrentThread::tid(), x);
}

class Foo
{
	public:
		explicit Foo(double x)
			: x_(x)
		{
		}

		void memberFunc()
		{
			printf("tid=%d, Foo::x_=%f\n", xnet::CurrentThread::tid(), x_);
		}

		void memberFunc2(const std::string& text)
		{
			printf("tid=%d, Foo::x_=%f, text=%s\n", xnet::CurrentThread::tid(), x_, text.c_str());
		}

	private:
		double x_;
};

int main()
{
	printf("pid=%d, tid=%d\n", ::getpid(), xnet::CurrentThread::tid());

	xnet::Thread t1(threadFunc);
	t1.start();
	t1.join();

	xnet::Thread t2(boost::bind(threadFunc2, 42),
				"thread for free function with argument");
	t2.start();
	t2.join();

	Foo foo(87.53);
	xnet::Thread t3(boost::bind(&Foo::memberFunc, &foo),
				"thread for member function without argument");
	t3.start();
	t3.join();

	xnet::Thread t4(boost::bind(&Foo::memberFunc2, boost::ref(foo), std::string("Testing...")));
	t4.start();
	t4.join();

	printf("number of created threads %d\n", xnet::Thread::numCreated());
}
