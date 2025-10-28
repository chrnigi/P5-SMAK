package dk.aau.chrinigin.Smak.web;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.service.GameService;

@RestController
public class GameController {
    private final GameService gameService;

    public GameController(GameService gameService) {
        this.gameService = gameService;
    }



    @GetMapping("/hello")
    public String hello() {
        return "Hello, Smak!";
    } 

    @GetMapping("/games")
    List<Game> getAllGames() {
        return gameService.getAll();
    }

    @GetMapping("/games/{id}")
    Game getGameById(@PathVariable Long id) {
        return gameService.get(id).orElseThrow(() -> new GameNotFoundException(id));
    }


    @PostMapping("games")
    Game addGame(@RequestBody Game game) {
        return gameService.save(game);
    }

}
