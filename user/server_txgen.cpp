/*
 * @file: server_txgen.cpp
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <iostream>
#include <exception>
#include <unistd.h>
#include <signal.h>
#include "server_txgen.h"

using namespace jsonrpc;
using namespace std;

static bool should_exit = false;
static void exit_handler(int sig)
{
    should_exit = true;
}

//method
void BaseServer::sayHello(const Json::Value &request, Json::Value &response)
{
    response = "Hello: " + request["name"].asString();
}

//notification
void BaseServer::notifyServer(const Json::Value &request)
{
    (void)request;
    cout << "server received some Notification" << endl;
}

Json::Value ZkgRpcServer::tx1to1Prove(const Json::Value &prove_data)
{
    string ask, s_r0, s_r1, r_apk, g, Gpk;
    uint64_t vpub_old, s_v0, s_v1, vpub_new, r_v;
    bool is_zero_cm0, is_zero_cm1;

    Tx1To1Param param;
    cm_pool_lock.lock();
    try
    {
        ask = prove_data["ask"].asString();
        vpub_old = prove_data["vpub_old"].asUInt64();
        s_v0 = prove_data["s_v0"].asUInt64();
        s_v1 = prove_data["s_v1"].asUInt64();
        s_r0 = prove_data["s_r0"].asString();
        s_r1 = prove_data["s_r1"].asString();
        is_zero_cm0 = prove_data["is_zero_cm0"].asBool();
        is_zero_cm1 = prove_data["is_zero_cm1"].asBool();
        r_apk = prove_data["r_apk"].asString();
        vpub_new = prove_data["vpub_new"].asUInt64();
        r_v = prove_data["r_v"].asUInt64();
        g = prove_data["g"].asString();
        Gpk = prove_data["Gpk"].asString();

        param = tx1to1->prove(
            cm_pool,
            ask,
            vpub_old,
            s_v0, s_v1,
            s_r0, s_r1,
            is_zero_cm0, is_zero_cm1,
            r_apk,
            vpub_new, r_v,
            g,
            Gpk);
    }
    catch (std::exception &e)
    {
        cerr << e.what() << endl;
        param.error_code = 1;
        param.description = e.what();
    }
    catch (...)
    {
        cerr << "Proof generate error" << endl;
    }
    cm_pool_lock.unlock();

    Json::Value ret, to_verifier, to_payee, to_myself;

    to_verifier["s_rts0"] = param.s_rts[0];
    to_verifier["s_rts1"] = param.s_rts[1];
    to_verifier["s_sns0"] = param.s_sns[0];
    to_verifier["s_sns1"] = param.s_sns[1];
    to_verifier["r_cms0"] = param.r_cms[0];
    to_verifier["r_cms1"] = param.r_cms[1];
    to_verifier["vpub_old"] = param.vpub_old;
    to_verifier["vpub_new"] = param.vpub_new;
    to_verifier["g"] = param.g;
    to_verifier["Gpk"] = param.Gpk;
    to_verifier["G_data"] = param.G_data;
    to_verifier["proof"] = param.proof;
    to_verifier["description"] = "Send me to verifier";

    to_payee["value"] = param.v_to_payee;
    to_payee["spend_key"] = param.v_to_payee_r;
    to_payee["description"] = "Use payee's ask to spend";

    to_myself["value"] = param.v_change;
    to_myself["spend_key"] = param.v_change_r;
    to_myself["description"] = "Use my ask to spend";

    ret["to_verifier"] = to_verifier;
    ret["to_payee"] = to_payee;
    ret["to_myself"] = to_myself;

    ret["error_code"] = param.error_code;
    ret["description"] = param.description;

    return ret;
}

void ZkgRpcServer::updateCmPool(const std::string &node_url)
{
    int64_t from = 0, to = -1;
    int query_cnt = 0;
    HttpClient client(node_url);
    Client node(client);
    while (!should_exit)
    {
        Json::Value params(Json::arrayValue);
        Json::Value param;
        param["from"] = from;
        param["to"] = to;
        params.append(param);

        Json::Value rsp;
        try
        {
            rsp = node.CallMethod("eth_getCmByRange", params);
            Json::Value &cms = rsp["cms"];
            cm_pool_lock.lock();
            {
                cout << "Query[" << query_cnt++ << "]:" << endl
                     << rsp << endl;
                for (int i = 0; i < (int)cms.size(); i++)
                {
                    if (from + i < (int)cm_pool->size())
                        cm_pool->set(from + i, cms[i].asString());
                    else
                        cm_pool->append(cms[i].asString());
                    cout << "update cm[" << cm_pool->size() - 1 << "]: " << cms[i].asString() << endl;
                }

                //If zero cm not set, set zero cm
                if (cm_pool->size() == 0)
                    cm_pool->append(Tx1To1API::ZERO_CM());
            }
            cm_pool_lock.unlock();
            from = rsp["to"].asInt64();
        }
        catch (JsonRpcException &e)
        {
            cerr << e.what() << endl;
        }

        printState();
        sleep(UPDATE_CM_EXPIRES);
    }
}

void ZkgRpcServer::printState()
{
    cout << "---------------------------------------------------------------" << endl;
    cout << "State: " << (tx1to1.get() == NULL ? "pk loading..." : "ready") << endl;
    cout << "Port:  " << this->port << endl;
    cout << "CM Pool size: " << cm_pool->size() << endl;

    if (cm_pool->size() > 0)
    {
        MerkleTree tree("debug cm tree");
        int cap = treeCapacity();
        int from = max(0, (int)cm_pool->size() - cap);
        cm_pool->for_each_cm_range(from, cm_pool->size() - 1,
                                   [&](std::string cm) {
                                       tree.append(cm);
                                   });
        cout << "CM Tree Root: " << tree.root() << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: ./server_txgen <this client port> <node rpc url> <pk file>" << endl;
        cout << "Eg:    ./server_txgen 9653 127.0.0.1:8545 pk.data" << endl;
        return 0;
    }

    int port = atoi(argv[1]);
    string url(argv[2]);
    string pk_file(argv[3]);
    try
    {
        HttpServer server(port);
        ZkgRpcServer client(server, port, url, true, false, pk_file, "vk.data");
        if (client.StartListening())
        {
            //register exit_handler signal
            signal(SIGABRT, &exit_handler);
            signal(SIGTERM, &exit_handler);
            signal(SIGINT, &exit_handler);

            cout << "Server ready" << endl;
            while (!should_exit)
                    sleep(1);
            //client.StopListening();
            //cout << "Server stop listening." << endl;
        }
        else
        {
            cout << "Error starting Server" << endl;
        }
    }
    catch (jsonrpc::JsonRpcException &e)
    {
        cerr << e.what() << endl;
    }

   return 0;
}