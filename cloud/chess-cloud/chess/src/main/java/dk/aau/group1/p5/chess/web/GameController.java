package dk.aau.group1.p5.chess.web;

import java.util.List;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PatchMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import dk.aau.group1.p5.chess.model.Game;
import dk.aau.group1.p5.chess.service.GameService;

@CrossOrigin(origins = "*")
@RestController
public class GameController {
    private final GameService gameService;

    public GameController(GameService gameService) {
        this.gameService = gameService;
    }

    @GetMapping("/games")
    public List<Game> getAllGames() {
        return gameService.getAll();
    }

    @GetMapping("/games/{id}")
    public Game getGameById(@PathVariable Long id) {
        return gameService.get(id).orElseThrow(() -> new GameNotFoundException(id)); 
    }

    @PostMapping("/games")
    public Game addGame(@RequestBody Game game) {
        return gameService.save(game);
    }

    @PatchMapping("/games/{id}")
    public Game updateGame(@PathVariable Long id, @RequestBody Game game) {
        Game g = getGameById(id);
        g.setGamestate(game.getGamestate()); 
        return gameService.save(g);
    } 
}
