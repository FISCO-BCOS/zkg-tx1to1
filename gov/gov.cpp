/*
 * @file: gov.cpp
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <iostream>
#include <circuit/zkg.hpp>

using namespace jsonrpc;
using namespace std;

void prinGovData(const Tx1To1GovInfo & info)
{
    cout << "----------------- Gov Info -----------------" << endl;
    cout << "vpub_old:\t" << info.vpub_old << endl;
    cout << "vpub_new:\t" << info.vpub_new << endl;
    cout << "in_apks[0]:\t" << info.in_apks[0] << endl;
    cout << "in_values[0]:\t" << info.in_values[0] << endl;
    cout << "in_apks[1]:\t" << info.in_apks[1] << endl;
    cout << "in_values[1]:\t" << info.in_values[1] << endl;
    cout << "out_apks[0]:\t" << info.out_apks[0] << endl;
    cout << "out_values[0]:\t" << info.out_values[0] << endl;
    cout << "out_apks[1]:\t" << info.out_apks[1] << endl;
    cout << "out_values[1]:\t" << info.out_values[1] << endl;
    cout << endl;
}

void queryGovData(const string &gsk, const string &url)
{
    int64_t from = 0, to = -1;
    HttpClient client(url);
    Client node(client);

    Json::Value params(Json::arrayValue);
    Json::Value param;
    param["from"] = from;
    param["to"] = to;
    params.append(param);

    Json::Value rsp;
    try
    {
        rsp = node.CallMethod("eth_getGovDataByRange", params);
        Json::Value &g_datas = rsp["G_datas"];
        {
            cout << "Query Gov data:" << endl
                 << rsp << endl;
            for (int i = 0; i < (int)g_datas.size(); i++)
            {
                cout << "Zkg Transaction[" << i << "]:" << endl;
                Tx1To1GovInfo G_tx_info = Tx1To1API::decrypt_tx_info(
                    gsk, //gsk
                    g_datas[i].asString());
                //prinGovData(G_tx_info);
            }
        }
    }
    catch (JsonRpcException &e)
    {
        cerr << e.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./govdata <gsk hex string> <node rpc url>" << endl;
        cout << "Eg:    ./govdata 12312432432 127.0.0.1:8545" << endl;
        return 0;
    }

    string gsk(argv[1]);
    string url(argv[2]);
    queryGovData(gsk, url);
    return 0;
}
