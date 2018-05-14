/*
 * @file: gpkgen.cpp
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <circuit/zkg.hpp>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        cout << "Usage: ./gpkgen <gsk hex string> <(optional) generator>" << endl;
        cout << "Eg:    ./gpkgen 123ef 333338d52b05df563667c6c87821f1fb54571e83925617a23f2541254e512c43" << endl;
        cout << "Get:   c4a0213fcea718a2f4ea19ae723c033f50b4bfdd8728836f10efdca0e1e647bc" << endl;
        return 0;
    }

    string ask(argv[1]);
    if (2 == argc)
    {
        cout << ZkgTool::generate_gpk(ask) << endl;
    }
    else if (3 == argc)
    {
        string g(argv[2]);
        cout << ZkgTool::generate_gpk(ask, g) << endl;
    }
    return 0;
}