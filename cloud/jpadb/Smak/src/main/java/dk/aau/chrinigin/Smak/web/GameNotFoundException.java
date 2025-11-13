package dk.aau.chrinigin.Smak.web;

// Custom exception to indicate that a game with the specified ID was not found.
public class GameNotFoundException extends RuntimeException {
    GameNotFoundException(Long id) {
        super(String.format("Game with id %d does not exist in database.", id));
    }
}


