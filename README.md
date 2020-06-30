# 限流器
1、概述：<br>
	基于令牌桶算法实现，实现qps限制。blog: https://mp.csdn.net/console/editor/html/106130057  
<br>
<br>
2、特点：<br>
（1）接口使用简单，无业务侵入，接入成本极低  
（2）线程安全，CPU友好，性能强劲  
（3）极轻量，核心代码150行  
<br>
<br>
3、编译：<br>
	进入src,make之后即可生成libratelimiter.so。<br>
<br>
<br>
4、使用：<br>
	example中提供示例，将libratelimiter.so放入/usr/lib/中后进入example，make即可生成可执行文件。下面这段代码将会执行5s：<br>
```
int main()
{
    RateLimiter r(100);//100qps限流器

    for(int i = 0; i < 500; ++i)
    {
        r.pass();//通过的速率为100/s
    }

    return 0;
}
```