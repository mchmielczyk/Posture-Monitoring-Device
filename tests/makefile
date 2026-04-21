GCC = gcc
CFLAGS = -Wall -pedantic -DFLAG_TEST_F_
LIB = Adxl345DriverTestRunner.c AllTests.c ../Core/Src/adxl345.c -IUnity/src Unity/src/unity.c -IUnity/extras/fixture/src Unity/extras/fixture/src/unity_fixture.c -IUnity/extras/memory/src -I../Core/Inc -IMocks
TARGET = Adxl345DriverTest
all: test
test:
	$(GCC) $(TARGET).c $(LIB) $(CFLAGS) -o $(TARGET)
	./Adxl345DriverTest.exe
clean:
	rm -f $(TARGET).exe