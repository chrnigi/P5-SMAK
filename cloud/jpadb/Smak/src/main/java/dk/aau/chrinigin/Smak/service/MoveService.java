package dk.aau.chrinigin.Smak.service;

import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.model.MoveId;
import dk.aau.chrinigin.Smak.repository.MoveRepository;
import org.springframework.stereotype.Service;
import java.util.List;
// Service layer for managing Move entities. Provides methods to retrieve all moves,
// get moves by game ID, save a move, and delete a move by its composite ID
@Service
public class MoveService {

    public static final int MAX_MOVES_PER_GAME = 600; 

    private final MoveRepository moveRepository;

    public MoveService(MoveRepository moveRepository) {
        this.moveRepository = moveRepository;
    }

    public List<Move> findAll() { return moveRepository.findAll(); }

    public List<Move> getByGameId(Long id) { return moveRepository.getByGameId(id); }

    public Move save(Move move) { return moveRepository.save(move); }

    public void deleteById(MoveId id) { moveRepository.deleteById(id); }
}
