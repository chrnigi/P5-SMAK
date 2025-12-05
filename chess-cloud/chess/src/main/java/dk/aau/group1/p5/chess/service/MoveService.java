package dk.aau.group1.p5.chess.service;

import java.util.List;

import org.springframework.stereotype.Service;

import dk.aau.group1.p5.chess.model.Move;
import dk.aau.group1.p5.chess.repository.MoveRepository;

@Service
public class MoveService {

    private final MoveRepository moveRepository;

    public MoveService(MoveRepository moveRepository) {
        this.moveRepository = moveRepository;
    }

    public List<Move> getAll() {
        return moveRepository.findAll();
    }

    public List<Move> getAllByGameId(Long id) {
        return moveRepository.getByGameId(id);
    }

    public Move saveMove(Move move) {
        return moveRepository.save(move);
    }

}