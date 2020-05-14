
TARGET =rate_limiter_example
CC =g++ -L../src -g -O3 -Wall -std=c++11
INCLUDE =$(shell find ./ -name "*.h")
SOURCE =$(shell find ./ -name "*.cpp")
OBJS =$(SOURCE:%.cpp=%.o)
LIBS =-lratelimiter
CFLAGS =-I../src

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.cpp $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
	
all:$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)
