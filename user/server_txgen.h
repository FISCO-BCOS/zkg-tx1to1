/*
 * @file: server_txgen.h
 * @author: jimmyshi 
 * @date: 4th May 2018
 * @copyright: MIT license (see LICENSE file)
 */


#include <cstdio>
#include <thread>
#include <string>
#include <iostream>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <jsonrpccpp/server/connectors/unixdomainsocketserver.h>
#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <circuit/zkg.hpp>

class BaseServer : public jsonrpc::AbstractServer<BaseServer>
{
  public:
    BaseServer(jsonrpc::HttpServer &server) : jsonrpc::AbstractServer<BaseServer>(server)
    {
        this->bindAndAddMethod(jsonrpc::Procedure("sayHello", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_STRING, "name", jsonrpc::JSON_STRING, NULL), &BaseServer::sayHello);
        this->bindAndAddNotification(jsonrpc::Procedure("notifyServer", jsonrpc::PARAMS_BY_NAME, NULL), &BaseServer::notifyServer);
        this->bindAndAddMethod(jsonrpc::Procedure("tx1to1_prove", jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_OBJECT, "prove_data", jsonrpc::JSON_OBJECT, NULL), &BaseServer::tx1to1ProveI);
    }

    //method
    void sayHello(const Json::Value &request, Json::Value &response);

    //notification
    void notifyServer(const Json::Value &request);

    inline void tx1to1ProveI(const Json::Value &request, Json::Value &response)
    {
        response = this->tx1to1Prove(request["prove_data"]);
    }

    virtual Json::Value tx1to1Prove(const Json::Value &prove_data) = 0;
};

class ZkgRpcServer : public BaseServer
{
  private:
    std::shared_ptr<Tx1To1API> tx1to1;
    std::shared_ptr<CMPool> cm_pool;
    boost::mutex cm_pool_lock;

    const int UPDATE_CM_EXPIRES = 5; // second
    int port;

  public:
    ZkgRpcServer(
        jsonrpc::HttpServer &server,
        int port,
        const std::string &node_url,
        bool is_preload_pk = true,
        bool is_preload_vk = false,
        const std::string &pk_file = "pk.data",
        const std::string &vk_file = "vk.data") : BaseServer(server), port(port)
    {
        cm_pool = std::make_shared<CMPool>();
        threadCallCmUpdate(node_url);

        tx1to1 = std::make_shared<Tx1To1API>(is_preload_pk, is_preload_vk, pk_file, vk_file);
    }

    virtual ~ZkgRpcServer()
    {
        this->StopListening();
        std::cout << "Server exit." << std::endl;
    }

    void updateCmPool(const std::string &node_url);

    Json::Value tx1to1Prove(const Json::Value &prove_data);

    std::shared_ptr<CMPool> aroundCmPool(const std::string &cm);

    void printState();

  private:
    void threadCallCmUpdate(const std::string &node_url)
    {
        std::thread threadUpdateCmPool(&ZkgRpcServer::updateCmPool, this, node_url);
        threadUpdateCmPool.detach();
    }
    static size_t treeCapacity()
    {
        return size_t(1 << Tx1To1API::TREE_DEPTH());
    }
};
