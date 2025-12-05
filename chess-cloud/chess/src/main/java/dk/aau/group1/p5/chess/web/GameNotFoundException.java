package dk.aau.group1.p5.chess.web;

public class GameNotFoundException extends RuntimeException {
    GameNotFoundException(Long id) {
        super(String.format("Game with id %d does not exist in the database.", id));
    }
}
