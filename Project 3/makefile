# the compiler
CC = gcc
CXX = g++


# forces removal without prompt
RM = rm -f

# compiler flags:
# -g debugging information to executable
# -Wall most compiler warnings enabled
# -Werror turns warnings into errors
# -O3 optimiation level 3
# -pedantic warns about GCC spesific options
# -MMD & -MP makes automatic dependancy files
CPPFLAGS = -g -std=c++11 -Wall -Wextra -Werror -O3 -pedantic -MMD -MP -fopenmp

# linker flags
LDFLAGS = -g -Wall -fopenmp


# libraries to link into executable
LDLIBS = 

# build executable
TARGET = project3
CPP_FILES = main.cpp functions.cpp

SRCS = $(addprefix src/,$(notdir $(CPP_FILES)))
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))


TEST_TARGET = testit
TEST_FILES = tests.cpp catch.cpp functions.cpp
TEST_SRCS = $(addprefix src/,$(notdir $(TEST_FILES)))
TEST_OBJS = $(patsubst src/%.cpp,obj/%.o,$(TEST_SRCS))





all: $(TARGET) $(TEST_TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CPPFLAGS) -c -o $@ $<
	


test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	


.phony: all clean dist-clean test
clean:
	$(RM) $(patsubst %.o,%.d,$(OBJS))
	$(RM) $(OBJS)
	$(RM) $(patsubst %.o,%.d,$(TEST_OBJS))
	$(RM) $(TEST_OBJS)

dist-clean: clean
	$(RM) $(TARGET)
	$(RM) $(TEST_TARGET)
	@rmdir --ignore-fail-on-non-empty obj 




