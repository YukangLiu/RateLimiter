//@author liuyukang

#include "rate_limiter.h"
#include "spinlock_guard.h"

#include <sys/time.h>
#include <float.h>
#include <unistd.h>

#define RETRY_IMMEDIATELY_TIMES 100//不睡眠的最大重试获得令牌的次数

 //qps限制最大为十亿
RateLimiter::RateLimiter(int64_t qps) : 
    bucketSize_(1), tokenLeft_(0), supplyUnitTime_(NS_PER_SECOND / qps), lastAddTokenTime_(0)
{ 
    assert(qps <= NS_PER_SECOND);
	assert(qps >= 0);
    lastAddTokenTime_ = now();
}

int64_t RateLimiter::now()
{
	struct timeval tv;
	::gettimeofday(&tv, 0);
	int64_t seconds = tv.tv_sec;
	return seconds * NS_PER_SECOND + tv.tv_usec * NS_PER_USECOND;
}

//对外接口，能返回说明流量在限定值内
void RateLimiter::pass()
{
	return mustGetToken();
}

//尝试获得令牌
//成功获得则返回true
//失败则返回false
bool RateLimiter::tryGetToken()
{
    supplyTokens();

	//获得一个令牌
	auto token = tokenLeft_.fetch_add(-1);
	if(token <= 0)
	{//已经没有令牌了，归还透支的令牌
		tokenLeft_.fetch_add(1);
		return false;
	}
	
    return true;
}

//必定成功获得令牌
//其中会进行重试操作
void RateLimiter::mustGetToken()
{
	bool isGetToken = false;
	for(int i = 0; i < RETRY_IMMEDIATELY_TIMES; ++i)
	{
		isGetToken =  tryGetToken();
		if(isGetToken)
		{
			return;
		}
	}

	while(1)
	{
		isGetToken =  tryGetToken();
		if(isGetToken)
		{
			return;
		}
		else
		{
			//让出CPU
			sleep(0);
		}
	}
}

void RateLimiter::supplyTokens()
{
	auto cur = now();
	if (cur - lastAddTokenTime_ < supplyUnitTime_)
	{
		return;
	}

	{
		SpinlockGuard lock(lock_);
		//等待自旋锁期间可能已经补充过令牌了
		int64_t newTokens = (cur - lastAddTokenTime_) / supplyUnitTime_;
		if (newTokens <= 0)
		{
			return;
		}
		
		//更新补充时间,不能直接=cur，否则会导致时间丢失
		lastAddTokenTime_ += (newTokens * supplyUnitTime_);
		
		auto freeRoom = bucketSize_ - tokenLeft_.load();
		if(newTokens > freeRoom || newTokens > bucketSize_)
		{
			newTokens = freeRoom > bucketSize_ ? bucketSize_ : freeRoom;
		}
		
		tokenLeft_.fetch_add(newTokens);
	}
	
}