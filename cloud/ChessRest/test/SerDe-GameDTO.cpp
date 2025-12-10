#include <oatpp/core/Types.hpp>
#include <cassert>
#include <clientlib.hpp>
#include <models.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>   

#define GAME_JSON_INPUT "{\"id\":1,\"gamestate\":\"WHITE_WIN\",\"gamestart\":\"2002-30-09\"}"

int main(void) {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

    std::string input(GAME_JSON_INPUT);

    auto dto = smak::models::GameDTO::createShared();
    dto->id = 1;
    dto->gamestate = smak::models::GameStateEnum::WHITE_WIN;
    dto->gamestart = "2002-30-09";
    
// Test deserialization

    auto dto_from_str = objectMapper->readFromString<oatpp::Object<smak::models::GameDTO>>(input);
    
    assert(dto->id == dto_from_str->id);
    assert(dto->gamestate == dto_from_str->gamestate);
    assert(dto->gamestart == dto_from_str->gamestart);

// Test serialization

    auto serialized = objectMapper->writeToString(dto);

    assert(serialized == GAME_JSON_INPUT);

    return 0;
    
}
