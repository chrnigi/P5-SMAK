#pragma once
#ifndef ENDPOINT_COMPONENTS_HPP
#define ENDPOINT_COMPONENTS_HPP

#include "oatpp/core/data/mapping/ObjectMapper.hpp"
#include "oatpp/network/Address.hpp"
#include "oatpp/network/ConnectionHandler.hpp"
#include "oatpp/network/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include <memory>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/core/macro/component.hpp>

namespace smak::controller {

class EndpointComponents {
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider) ([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8080, oatpp::network::Address::IP_4});
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter) ([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler) ([]{
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectManager)([] {
        return oatpp::parser::json::mapping::ObjectMapper::createShared();
    }());

};

}
#endif