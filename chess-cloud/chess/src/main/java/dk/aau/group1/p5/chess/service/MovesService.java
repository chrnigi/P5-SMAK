package dk.aau.group1.p5.chess.service;

import java.util.List;

import org.springframework.stereotype.Service;

import dk.aau.group1.p5.chess.model.Moves;
import dk.aau.group1.p5.chess.repository.MovesRepository;

@Service
public class MovesService {

    private final MovesRepository movesRepository;

    public MovesService(MovesRepository movesRepository) {
        this.movesRepository = movesRepository;
    }

    public List<Moves> getAll() {
        return movesRepository.findAll();
    }

    public List<Moves> getAllByGameId(Long id) {
        return movesRepository.getByGameId(id);
    }

    public Moves saveMove(Moves move) {
        return movesRepository.save(move);
    }

}