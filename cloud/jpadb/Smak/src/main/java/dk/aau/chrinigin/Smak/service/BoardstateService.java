package dk.aau.chrinigin.Smak.service;

import java.util.List;

import org.springframework.stereotype.Service;

import dk.aau.chrinigin.Smak.model.Boardstate;
import dk.aau.chrinigin.Smak.repository.BoardstateRepository;

@Service
public class BoardstateService {
    
    private final BoardstateRepository boardstateRepository;

    public BoardstateService(BoardstateRepository boardstateRepository) {
        this.boardstateRepository = boardstateRepository;
    }

    public List<Boardstate> getAll() {
        return boardstateRepository.findAll();
    }

    public List<Boardstate> getAllByGameId(Long id) {
        return boardstateRepository.getByGameId(id);    
    }

    public Boardstate addBoardstate(Boardstate boardstate) {
        return boardstateRepository.addBoardstate(
            boardstate.getId(), 
            boardstate.getX_placement(),
            boardstate.getO_placement(),
            boardstate.getMoveno()
        ); 
    }
   
    public Boardstate saveBoardstate(Boardstate boardstate) {
        System.out.println(boardstate);
        return boardstateRepository.save(boardstate);
    }
}
