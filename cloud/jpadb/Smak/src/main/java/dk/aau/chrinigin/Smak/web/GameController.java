package dk.aau.chrinigin.Smak.web;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.ResultEnum;
import dk.aau.chrinigin.Smak.repository.GameRepository;
import dk.aau.chrinigin.Smak.service.MoveService;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.http.HttpStatus;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// Handles all game level operations: creating games, listing games, etc.  
// Fetching a game's current state (FEN, PGN, result), and retrieving a single game.
// Example: games/1/state gives you the first game FEN, pgn + game result.

@RestController
public class GameController {

    private final GameRepository repository;
    private final MoveService moveService;

    public GameController(GameRepository repository, MoveService moveService) {
        this.repository = repository;
        this.moveService = moveService;
    }

    @GetMapping("/hello")
    String hello() { return "Hello, Smak!"; }

    @GetMapping("/games")
    List<Game> all() { return repository.findAll(); }

    @PostMapping("/games")
    Game newGame(@RequestBody Game g) {
        if (g.getGamestart() == null) g.setGamestart(new Timestamp(System.currentTimeMillis()));
        if (g.getInitialFen() == null || g.getInitialFen().isBlank())
            g.setInitialFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        if (g.getResult() == null) g.setResult(ResultEnum.INPROGRESS);
        return repository.save(g);
    }
 
    @GetMapping("/games/{id}")
    Game one(@PathVariable Long id) {
        return repository.findById(id).orElseThrow(() ->
                new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + id));
    }

    @GetMapping("/games/{id}/state")
    Map<String, Object> state(@PathVariable Long id) {
     Game g = repository.findById(id).orElseThrow(() ->
            new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + id));
        var moves = moveService.getByGameId(id);
        String fen = (moves == null || moves.isEmpty())
            ? g.getInitialFen()
            : moves.get(moves.size() - 1).getFenAfter();

        Map<String, Object> out = new HashMap<>();
        out.put("gameId", id);
        out.put("fen", fen);
        out.put("result", g.getResult());
        out.put("pgn", g.getPgn());
        out.put("plies", moves == null ? 0 : moves.size());
        out.put("gamestart", g.getGamestart());
        out.put("gameend", g.getGameend());
        return out;
    }


    @GetMapping("/games/{id}/pgn")
    String pgn(@PathVariable Long id) {
        Game g = repository.findById(id).orElseThrow(() ->
                new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + id));
        return g.getPgn() == null ? "" : g.getPgn();
    }
}
