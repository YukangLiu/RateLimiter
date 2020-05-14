//@author Liu Yukang
#pragma once
#include "spinlock.h"
#include "utils.h"

//���std::atomic_int����Ķ�Ԫ�ź���ʹ�ã�Ϊ1��ʾ��Դ����ʹ�ã�Ϊ0��ʾ��Դ����ʹ��
class SpinlockGuard
{
public:
	SpinlockGuard(Spinlock& l)
		: lock_(l)
	{
		lock_.lock();
	}

	~SpinlockGuard()
	{
		lock_.unlock();
	}

	DISALLOW_COPY_MOVE_AND_ASSIGN(SpinlockGuard);

private:
	Spinlock& lock_;

};

