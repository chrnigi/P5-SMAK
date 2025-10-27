package dk.aau.chrinigin.Smak.service;

import java.util.List;

import org.springframework.stereotype.Service;

import dk.aau.chrinigin.Smak.model.Boardstate;
import dk.aau.chrinigin.Smak.repository.BoardstateRepository;

@Service
public class BoardstateService {
    
    static int MAX_BOARDSTATES_PER_GAME = 9;
    private final BoardstateRepository boardstateRepository;

    public BoardstateService(BoardstateRepository boardstateRepository) {
        this.boardstateRepository = boardstateRepository;
    }

    public List<Boardstate> getAll() {
        return boardstateRepository.findAll();
    }

    public List<Boardstate> getAllByGameId(Long id) {

        // for (int i = 0; i < MAX_BOARDSTATES_PER_GAME; i++) {
        //     ids.get(i).setId(id);
        // }

        return boardstateRepository.getByGameId(id);
    
    }
   
    public Boardstate saveBoardstate(Boardstate boardstate) {
        return boardstateRepository.save(boardstate);
    }
}
