package dk.aau.group1.p5.chess.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import dk.aau.group1.p5.chess.model.Games;

public interface GamesRepository extends JpaRepository<Games, Long> {
    
}
