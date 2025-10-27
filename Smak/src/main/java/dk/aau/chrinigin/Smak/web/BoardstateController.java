package dk.aau.chrinigin.Smak.web;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import dk.aau.chrinigin.Smak.model.Boardstate;
import dk.aau.chrinigin.Smak.service.BoardstateService;

@RestController
public class BoardstateController {
    
    private final BoardstateService boardstateService;

    public BoardstateController(BoardstateService boardstateService) {
        this.boardstateService = boardstateService;
    }

    @GetMapping("/boardstates")
    public List<Boardstate> getAll() {
        return boardstateService.getAll();
    }

    @GetMapping("/boardstates/{id}")
    public List<Boardstate> getByGameId(@PathVariable Long id) {
        return boardstateService.getAllByGameId(id);
    }

    @PostMapping("/boardstates")
    public Boardstate addBoardstate(@RequestBody Boardstate boardstate) {
        return boardstateService.saveBoardstate(boardstate);
    }
}
