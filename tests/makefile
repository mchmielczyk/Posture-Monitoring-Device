CC = gcc
CXX = g++
CFLAGS = -Wall -Wpedantic -I../Core/Inc -Icpputest/include -IMocks
CXXFLAGS = -Wall -Wpedantic -I../Core/Inc -Icpputest/include -IMocks
LIB_DIR = -Lcpputest/cpputest_build/src/CppUTest -Lcpputest/cpputest_build/src/CppUTestExt
LIBS = -lCppUTest -lCppUTestExt

C_SOURCES = ../Core/Src/adxl345.c Mocks/fake_adxl345.c Mocks/adxl345spy.c 
CPP_SOURCES = AllTests.cpp Adxl345DriverTest.cpp Adxl345DriverTestSpy.cpp

C_OBJS = $(C_SOURCES:.c=.o)
CPP_OBJS = $(CPP_SOURCES:.cpp=.o)

all: AllTests

AllTests: $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CPP_OBJS) $(C_OBJS) $(LIB_DIR) $(LIBS) -o $@
	./$@.exe -v

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@