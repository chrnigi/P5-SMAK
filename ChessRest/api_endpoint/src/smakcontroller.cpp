#include "oatpp/core/base/Environment.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionHandler.hpp"
#include "oatpp/network/ConnectionProvider.hpp"
#include "oatpp/network/Server.hpp"
#include <smakcontroller.hpp>
#include <EndpointComponents.hpp>
#include <string>

namespace smak::controller {

void run_server() {
    using namespace oatpp;
    
    EndpointComponents endpointComponents;

    OATPP_COMPONENT(std::shared_ptr<web::server::HttpRouter>, router);
    auto smakController = std::make_shared<SmakController>();
    router->addController(smakController);

    OATPP_COMPONENT(std::shared_ptr<network::ConnectionHandler>, connectionHandler);

    OATPP_COMPONENT(std::shared_ptr<network::ServerConnectionProvider>, connectionProvider);

    network::Server server(connectionProvider, connectionHandler);
    

    OATPP_LOGI(__PRETTY_FUNCTION__, " Server running on port %s", (char *)connectionProvider->getProperty("port").getData())
    server.run();

}


}/* namespace smak::controller */
