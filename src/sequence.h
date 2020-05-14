//@Author LiuYukang
#pragma once
#include <atomic>

#define CACHELINE_SIZE_BYTES 256
#define CACHELINE_PADDING_FOR_ATOMIC_INT64_SIZE (CACHELINE_SIZE_BYTES - sizeof(std::atomic_int64_t))
#define CACHELINE_PADDING_FOR_INT64_SIZE (CACHELINE_SIZE_BYTES - sizeof(int64_t))

//��std::atomic_int64_t�����˷�װ���ڴ油�뱣֤_seq��һ����������
class AtomicSequence
{
public:
	AtomicSequence(int64_t num = 0L) : _seq(num) {};
	~AtomicSequence() {};
	AtomicSequence(const AtomicSequence&) = delete;
	AtomicSequence(const AtomicSequence&&) = delete;
	void operator=(const AtomicSequence&) = delete;

	void store(const int64_t val)//, std::memory_order _order = std::memory_order_seq_cst)
	{
		_seq.store(val);//,_order);
	}

	int64_t load()//std::memory_order _order = std::memory_order_seq_cst)
	{
		return _seq.load();// _order);
	}

	int64_t fetch_add(const int64_t increment)//, std::memory_order _order = std::memory_order_seq_cst)
	{
		return _seq.fetch_add(increment);// _order);
	}

private:
	//���߶����룬�Ա�֤_seq������������������һ��������
	char _frontPadding[CACHELINE_PADDING_FOR_ATOMIC_INT64_SIZE];
	std::atomic_int64_t _seq;
	char _backPadding[CACHELINE_PADDING_FOR_ATOMIC_INT64_SIZE];
};

//��int64_t�����˷�װ���ڴ油�뱣֤_seq��һ����������
class Sequence
{
public:
	Sequence(int64_t num = 0L) : _seq(num) {};
	~Sequence() {};
	Sequence(const Sequence&) = delete;
	Sequence(const Sequence&&) = delete;
	void operator=(const Sequence&) = delete;

	void store(const int64_t val)
	{
		_seq = val;
	}

	int64_t load()
	{
		return _seq;
	}

private:
	//���߶����룬�Ա�֤_seq������������������һ��������
	char _frontPadding[CACHELINE_PADDING_FOR_INT64_SIZE];
	volatile int64_t _seq;
	char _backPadding[CACHELINE_PADDING_FOR_INT64_SIZE];
};