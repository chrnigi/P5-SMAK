package dk.aau.chrinigin.Smak.model;

/**
 * Minimal piece encoding. We only need:
 * - which side (white/black)
 * - which type (pawn, knight, bishop, rook, queen, king)
 * - or NONE if there was no piece
 */
public enum PieceType {
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,

    NONE
}
