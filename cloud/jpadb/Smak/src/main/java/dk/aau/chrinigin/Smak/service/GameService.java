package dk.aau.chrinigin.Smak.service;
// Service layer for managing Game entities. Provides methods to retrieve all games,
// get a specific game by ID, and save a game to the database.

import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.repository.GameRepository;

@Service
public class GameService {
    private final GameRepository gameRepository;

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
