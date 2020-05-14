//@Author Liu Yukang 
#pragma once
#include <atomic>
#include "utils.h"

//���std::atomic_int����Ķ�Ԫ�ź���ʹ�ã�Ϊ1��ʾ��Դ����ʹ�ã�Ϊ0��ʾ��Դ����ʹ��
class Spinlock
{
public:
	Spinlock()
		: sem_(1)
	{ }

	~Spinlock() { unlock(); }

	DISALLOW_COPY_MOVE_AND_ASSIGN(Spinlock);

	void lock()
	{
		int exp = 1;
		while (!sem_.compare_exchange_strong(exp, 0))
		{
			exp = 1;
		}
	}

	void unlock()
	{
		sem_.store(1);
	}

private:
	std::atomic_int sem_;

};
