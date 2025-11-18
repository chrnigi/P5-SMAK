package dk.aau.group1.p5.chess.web;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import dk.aau.group1.p5.chess.model.Moves;
import dk.aau.group1.p5.chess.service.MovesService;

public class MovesController {
    private final MovesService movesService;

    public MovesController(MovesService movesService) {
        this.movesService = movesService;
    }

    @GetMapping("/moves")
    public List<Moves> getAll() {
        return movesService.getAll();
    }

    @GetMapping("/moves/{id}")
    public List<Moves> getByGameId(@PathVariable Long id) {
        return movesService.getAllByGameId(id);
    }

    @PostMapping("/moves")
    public Moves addMove(@RequestBody Moves move) {
        return movesService.saveMove(move);
    }


}
