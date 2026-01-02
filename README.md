## Compile the compiler (outputs c_entry.s)
g++ -g compile_program.cpp -o compile_program

## Create object file
as -arch arm64 -o c_entry.o c_entry.s

## Compile & run test driver (reference _c_entry())
g++ -g test_driver.cpp c_entry.o -o test_driver

