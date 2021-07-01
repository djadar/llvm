# llvm
llvm pass to obtain vulnerable buffer list

## To obtain llvm source code that we used

git clone https://github.com/llvm-mirror/llvm.git 

## To compile it
cd build/
cmake ../

# To refister a pass
cmake --build .

# For testing our pass
clang -emit-llvm -c hello.c
cd build/ ./bin/opt -load lib/LLVMOurPass.so -OurPass <hello.bc> /dev/null


