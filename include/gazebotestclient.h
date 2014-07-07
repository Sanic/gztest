/**
 * THIS FILE IS GENERATED BY jsonrpcstub, DO NOT CHANGE IT!!!!!
 */

#ifndef _GAZEBOTESTCLIENT_H_
#define _GAZEBOTESTCLIENT_H_

#include <jsonrpc/rpc.h>

class GazeboTestClient
{
    public:
        GazeboTestClient(jsonrpc::AbstractClientConnector* conn)
        {
            this->client = new jsonrpc::Client(conn);
        }
        ~GazeboTestClient()
        {
            delete this->client;
        }

        bool onObject(const std::string& object, const std::string& surface) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["object"] = object; 
p["surface"] = surface; 

            Json::Value result = this->client->CallMethod("onObject",p);
    if (result.isBool())
        return result.asBool();
     else 
         throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());

        }

        void resetWorld() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            this->client->CallNotification("resetWorld",p);
        }

    private:
        jsonrpc::Client* client;
};
#endif //_GAZEBOTESTCLIENT_H_
