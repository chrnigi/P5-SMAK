package dk.aau.group1.p5.chess.service;

import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;

import dk.aau.group1.p5.chess.model.Game;
import dk.aau.group1.p5.chess.repository.GameRepository;

@Service
public class GameService {
    private final GamesRepository gameRepository;

    public GameService(GameRepository gameRepository) {
        this.gameRepository = gameRepository;
    }

    public List<Game> getAll() {
        return gameRepository.findAll();
    }

    public Optional<Game> get(Long id) {
        return gameRepository.findById(id);
    }

    public Game save(Game game) {
        return gameRepository.save(game);
    }

}
