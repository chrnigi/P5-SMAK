package dk.aau.chrinigin.Smak.web;


public class GameNotFoundException extends RuntimeException {
    GameNotFoundException(Long id) {
        super(String.format("Game with id %d does not exist in database.", id));
    }
}


