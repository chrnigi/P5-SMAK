package dk.aau.group1.p5.chess.web;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PatchMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import dk.aau.group1.p5.chess.model.Games;
import dk.aau.group1.p5.chess.service.GamesService;

@RestController
public class GamesController {
    private final GamesService gamesService;

    public GamesController(GamesService gamesService) {
        this.gamesService = gamesService;
    }

    @GetMapping("/games")
    public List<Games> getAllGames() {
        return gamesService.getAll();
    }

    @GetMapping("/games/{id}")
    public Games getGameById(@PathVariable Long id) {
        return gamesService.get(id).orElseThrow(() -> new RuntimeException() /* Change to custom exception */); 
    }

    @PostMapping("/games")
    public Games addGame(@RequestBody Games game) {
        return gamesService.save(game);
    }

    @PatchMapping("/games/{id}")
    public Games updateGame(@PathVariable Long id, @RequestBody Games game) {
        game.setId(id);
        return gamesService.save(game);
    } 
}
