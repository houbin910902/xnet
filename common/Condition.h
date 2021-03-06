// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

#ifndef _XNET_COMMON_CONDITION_H_
#define _XNET_COMMON_CONDITION_H_

#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace xnet
{

class Condition: boost::noncopyable
{
public:
	explicit Condition(MutexLock& mutex) :
					mutex_(mutex)
	{
		pthread_cond_init(&pcond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	bool waitForSeconds(int seconds);

	void notify()
	{
		pthread_cond_signal(&pcond_);
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}
#endif  // _XNET_COMMON_CONDITION_H_
