
### Goal
- CMake 를 이용한 C/C++ 빌드


### Note
- vscode extention: C/C++, CMake, CMake Tools, cmake-format

### 참고 사이트
- https://github.com/jacking75/examples_CMake
- https://goodgodgd.github.io/ian-flow/archivers/vscode-tutorial

### Test
```
➜  cmake_build git:(main) ✗ ./build_cmake.sh
cmake version 3.10.2

CMake suite maintained and supported by Kitware (kitware.com/cmake).
DK_ENV1=1000
DK_ENV2=DK_TEST_ENV
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/dk/myCpp/cmake_build/.build
Scanning dependencies of target test-cmake
[ 50%] Building CXX object CMakeFiles/test-cmake.dir/main.cpp.o
[100%] Linking CXX executable test-cmake
[100%] Built target test-cmake
./test-cmake
arg1
arg2
1000
DK_TEST_ENV
hello world: it's me, dk
```