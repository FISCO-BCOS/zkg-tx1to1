/*
 * @file: apkgen.cpp
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <circuit/zkg.hpp>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./apkgen <ask hex string>" << endl;
        cout << "Eg:    ./apkgen 123ef" << endl;
        cout << "Get:   2a4fdf9331f481329f01f21c928b032e1542004e93af96bf3674ab9957a0c826" << endl;
        return 0;
    }

    string ask(argv[1]);
    cout << ZkgTool::generate_apk(ask) << endl;
    return 0;
}