#include <oatpp/core/base/Environment.hpp>
#include <smakcontroller.hpp>
#include <clientlib.hpp>

int main(void) { 

    oatpp::base::Environment::init();


    smak::controller::run_server();

    oatpp::base::Environment::destroy();

    return 0; 
}

