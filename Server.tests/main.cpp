#include <System/SystemTests>
#include <Network/NetworkTests>
#include <Database/DatabaseTests>
#include <iostream>

using namespace MafiaTests;


int main(int argc, char* argv[]){
	SystemTests::test(argc, argv);
	NetworkTests::test(argc, argv);
	DatabaseTests::test(argc, argv);
    std::cout << "Total tests failed: " << totalTestsFailed << std::endl;
    return 0;
}
