//@author liuyukang
#include "../src/rate_limiter.h"

#include <iostream>
#include <sys/time.h>
#include <unistd.h>

int64_t now()
{
	struct timeval tv;
	::gettimeofday(&tv, 0);
	int64_t seconds = tv.tv_sec;
	return seconds * NS_PER_SECOND + tv.tv_usec * NS_PER_USECOND;
}

int main()
{
    RateLimiter r(100);//100qps限流器
    sleep(1);

    auto start = now();

    for(int i = 0; i < 500; ++i)
    {
        r.pass();//通过的速率为100/s
    }

    auto end = now();
    std::cout << (end - start) / 1000000 << "ms" << std::endl;//此时打印的时间为5000ms
    return 0;
}