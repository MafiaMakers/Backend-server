#include <System/SystemTests>

#include <iostream>

using namespace MafiaTests;


int main(int argc, char* argv[]){
	SystemTests::test(argc, argv);

    std::cout << "Total tests failed: " << totalTestsFailed << std::endl;
    return 0;
}
