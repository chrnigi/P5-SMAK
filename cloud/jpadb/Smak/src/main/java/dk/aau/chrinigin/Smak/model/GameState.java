package dk.aau.chrinigin.Smak.model;

/**
 * Overall game state:
 * - PRE_GAME: no moves yet
 * - WHITE_TO_MOVE / BLACK_TO_MOVE: game in progress
 * - WHITE_WIN / BLACK_WIN / DRAW: finished games
 * - ILLEGAL_STATE: fallback if something goes wrong
 */
public enum GameState {
    PRE_GAME,
    WHITE_TO_MOVE,
    BLACK_TO_MOVE,
    WHITE_WIN,
    BLACK_WIN,
    DRAW,
    ILLEGAL_STATE
}
