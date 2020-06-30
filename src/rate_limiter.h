//@author liuyukang
#pragma once

#include "sequence.h"
#include "spinlock.h"

#include <assert.h>

#define NS_PER_SECOND 1000000000//一秒的纳秒数
#define NS_PER_USECOND 1000//一微秒的纳秒数

//限流器
//最大qps为1,000,000,000,最小为1
//使用：
//  RateLimiter r(100);
//  r.pass();
//  能通过r.pass()函数即可保证流速
//特点：
//  1、接口使用简单，无业务侵入，接入成本极低
//  2、线程安全，CPU友好，性能强劲
//  3、极轻量，核心代码150行
//原理：
//  基于令牌桶算法实现
class RateLimiter 
{
public:
    //qps限制最大为十亿
    RateLimiter(int64_t qps);

    DISALLOW_COPY_MOVE_AND_ASSIGN(RateLimiter);

    //对外接口，能返回说明流量在限定值内
    void pass();

private:

    //获得当前时间，单位ns
    int64_t now();

    //更新令牌桶中的令牌
    void supplyTokens();

    //尝试获得令牌
    //成功获得则返回true
    //失败则返回false
    bool tryGetToken();

    //必定成功获得令牌
    //其中会进行重试操作
    void mustGetToken();

    //令牌桶大小
    const int64_t bucketSize_;
    
    //剩下的token数
    AtomicSequence tokenLeft_;

    //补充令牌的单位时间
    const int64_t supplyUnitTime_;

    //上次补充令牌的时间，单位纳秒
    int64_t lastAddTokenTime_;

    //自旋锁
    Spinlock lock_;
};