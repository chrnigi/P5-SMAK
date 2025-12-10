package dk.aau.group1.p5.chess.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import dk.aau.group1.p5.chess.model.Game;

public interface GameRepository extends JpaRepository<Game, Long> {
    
}
