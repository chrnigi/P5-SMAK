package dk.aau.group1.p5.chess.service;

import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;

import dk.aau.group1.p5.chess.model.Games;
import dk.aau.group1.p5.chess.repository.GamesRepository;

@Service
public class GamesService {
    private final GamesRepository gamesRepository;

    public GamesService(GamesRepository gamesRepository) {
        this.gamesRepository = gamesRepository;
    }

    public List<Games> getAll() {
        return gamesRepository.findAll();
    }

    public Optional<Games> get(Long id) {
        return gamesRepository.findById(id);
    }

    public Games save(Games game) {
        return gamesRepository.save(game);
    }

}
