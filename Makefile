#
#
#
.PHONY: all


SRCFILES += flexi_type.cpp
SRCFILES += democle_types.cpp
SRCFILES += plan.cpp
SRCFILES += goal.cpp
SRCFILES += engine.cpp
SRCFILES += context.cpp
SRCFILES += condition.cpp
SRCFILES += agent.cpp
SRCFILES += democle.cpp
SRCFILES += messaging.cpp
SRCFILES += sensor.cpp
SRCFILES += sensor_lib.cpp

OBJS = $(SRCFILES:.cpp=.o)

CXXFLAGS = #-std=c++20

all: test_max sieve test_timer

sieve: $(OBJS) sieve.o
	$(CXX) $(CXXFLAGS) $(OBJS) sieve.o -o $@

test_max: $(OBJS) test_max.o
	$(CXX) $(CXXFLAGS) $(OBJS) test_max.o -o $@

test_timer: $(OBJS) test_timer.o
	$(CXX) $(CXXFLAGS) $(OBJS) test_timer.o -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) test_max.o sieve.o test_timer.o test_max sieve test_timer
