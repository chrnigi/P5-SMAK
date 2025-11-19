package dk.aau.chrinigin.Smak.web;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.GameState;
import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.repository.GameRepository;
import dk.aau.chrinigin.Smak.service.MoveService;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
public class GameController {

    private final GameRepository repository;
    private final MoveService moveService;

    public GameController(GameRepository repository, MoveService moveService) {
        this.repository = repository;
        this.moveService = moveService;
    }

    @GetMapping("/hello")
    String hello() {
        return "Hello, Smak!";
    }

    @GetMapping("/games")
    List<Game> all() {
        return repository.findAll();
    }

    @PostMapping("/games")
    Game newGame(@RequestBody Game g) {
        if (g.getGamestart() == null) {
            g.setGamestart(new Timestamp(System.currentTimeMillis()));
        }
        if (g.getState() == null) {
            g.setState(GameState.PRE_GAME);
        }
        return repository.save(g);
    }

    @GetMapping("/games/{id}")
    Game one(@PathVariable Long id) {
        return repository.findById(id).orElseThrow(() ->
                new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + id));
    }

    /**
     * Small helper endpoint that returns a summary for a game.
     * Useful if you want quick metadata (state, time, number of plies).
     */
    @GetMapping("/games/{id}/state")
    Map<String, Object> state(@PathVariable Long id) {
        Game g = repository.findById(id).orElseThrow(() ->
                new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + id));

        List<Move> moves = moveService.getByGameId(id);

        Map<String, Object> out = new HashMap<>();
        out.put("gameId", g.getId());
        out.put("gamestart", g.getGamestart());
        out.put("gameend", g.getGameend());
        out.put("state", g.getState());
        out.put("plies", moves == null ? 0 : moves.size());
        return out;
    }
}
