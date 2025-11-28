#include <chess.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
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

oatpp::Object<models::EvalDTO> evalToDto(Evaluation e) {
    oatpp::Object<models::EvalDTO> out;
    out->pawn_eval  = e.m_eval;
    int8_t b_from   = static_cast<int8_t>(e.getBestmove().from().index());
    int8_t b_to     = static_cast<int8_t>(e.getBestmove().to().index());
    out->bestmove   = (b_from << 8) & b_to;

    int8_t p_from   = static_cast<int8_t>(e.getPonder().from().index());
    int8_t p_to     = static_cast<int8_t>(e.getPonder().to().index());
    out->ponder     = (p_from << 8) & p_to;

    out->result     = static_cast<models::GameResultEnum>(e.res);
    out->reason     = static_cast<models::GameResultReasonEnum>(e.reason);    
    out->forced_mate = e.isMate();

    return out;
}


}/* namespace smak::controller */
