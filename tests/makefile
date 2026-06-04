CFLAGS= -Wall -pedantic
CXXFLAGS=  -Wall -pedantic
CXXTARGET= units/adxl345allTests
FAKES = fakes/fakeSTM.c spies/adxl345spy.c mocks/mockSTM.c
SOURCES =  ../Core/Src/adxl345.c units/adxl345driverTest.cpp
CXXINCLUDE =-Icpputest/include -Iunits -I../Core/Inc -Ispies/
CXXLIB= -Lcpputest/cpputest_build/src/CppUTest -Lcpputest/cpputest_build/src/CppUTestExt -lCppUTest -lCppUTestExt
all:

static testcpp:
	"C:/Program Files/Cppcheck/cppcheck.exe" --addon=misra --enable=all ../Core/Src/adxl345.c ../Core/Src/main.c -I../Core/Inc
testcpp:
	g++ $(CXXTARGET).cpp $(SOURCES) $(FAKES) $(CXXFLAGS) $(CXXINCLUDE) $(CXXLIB) -o build/testFile
	./build/testFile.exe -v

dynamic:
	del /Q build\*.o build\*.gcda build\*.gcno build\*.exe
	g++ --coverage -O0 -g $(CXXTARGET).cpp $(SOURCES) $(FAKES) $(CXXFLAGS) $(CXXINCLUDE) $(CXXLIB) -o build/testFile
	./build/testFile.exe -v
	gcovr -r .. 