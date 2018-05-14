/*
 * @file: pkvkgen.cpp
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <iostream>
#include <string>
#include "circuit/zkg.hpp"
using namespace std;

//int tx1to1_main()
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./pkvkgen <pk name> <vk name>" << endl;
        cout << "Eg:    ./pkvkgen pk.data vk.data" << endl;
        return 0;
    }

    string pk(argv[1]);
    string vk(argv[2]);
    //Generate pk, vk
    Tx1To1API::generate(pk, vk);
    return 0;
}