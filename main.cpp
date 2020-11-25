#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include "bst.h"

int main()
{
    // Initialize seed for randomness
    std::srand( static_cast<uint32_t>(std::time(nullptr)) );

    // Construct vector of random values
    std::vector<int> v_init(1);
    for ( auto& it : v_init ) { it = std::rand(); }

    BST::BST<int> test(2);

    test.insert(12);
    test.insert(-1);
    test.insert( 6);

    std::cout << "Test contains : " << ( test.contains(2)      ? "SUCCESS" : "FAILED" ) << std::endl;
    std::cout << "Test minVal   : " << ( (test.minVal() == -1) ? "SUCCESS" : "FAILED" ) << std::endl;
    std::cout << "Test maxVal   : " << ( (test.maxVal() == 12) ? "SUCCESS" : "FAILED" ) << std::endl;

    test.insert(v_init);
    test.insert( {1,3,5,7,9,10} );

    return 0;
}
