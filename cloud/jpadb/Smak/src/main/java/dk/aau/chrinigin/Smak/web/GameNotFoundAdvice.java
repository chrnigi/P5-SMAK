package dk.aau.chrinigin.Smak.web;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseStatus;
import org.springframework.web.bind.annotation.RestControllerAdvice;
// Advice to handle GameNotFoundException globally and return a 404 status code.
@RestControllerAdvice
public class GameNotFoundAdvice {
    @ExceptionHandler(GameNotFoundException.class) 
    @ResponseStatus(HttpStatus.NOT_FOUND)
    String gameNotFoundHandler(GameNotFoundException e) {
        return e.getMessage();
    }
}
