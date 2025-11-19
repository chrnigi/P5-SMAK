package dk.aau.group1.p5.chess.web;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import dk.aau.group1.p5.chess.model.Move;
import dk.aau.group1.p5.chess.service.MoveService;

public class MoveController {
    private final MoveService moveService;

    public MoveController(MoveService moveService) {
        this.moveService = moveService;
    }

    @GetMapping("/moves")
    public List<Move> getAll() {
        return moveService.getAll();
    }

    @GetMapping("/moves/{id}")
    public List<Move> getByGameId(@PathVariable Long id) {
        return moveService.getAllByGameId(id);
    }

    @PostMapping("/moves")
    public Move addMove(@RequestBody Move move) {
        return moveService.saveMove(move);
    }


}
