#include <chesslib.hpp>
#include <cassert>
#include <models.hpp>

int main(void) {
    auto dto = smak::models::MoveDTO::createShared();

    dto->id = 1;
    dto->move_number = 1;
    dto->ply_number = 1;
    dto->piece_moved = smak::models::PiecesEnum::PAWN;
    dto->piece_captured = nullptr;
    dto->is_promotion = false;
    dto->is_capture = false;
    dto->is_enpassant = false;
    dto->from_square = 12; // E2
    dto->to_square = 28; // E4

    auto simpledto = dto->to_simple();

    assert(simpledto.is_capture == false);
    assert(simpledto.is_enpassant == false);
    assert(simpledto.is_promotion == false);
    assert(simpledto.from == 12);
    assert(simpledto.to == 28);
    assert(simpledto.is_normal == true);
    assert(simpledto.type == smak::parsing::SimpleMoveDTO::NORMAL);

    return 0;

}
