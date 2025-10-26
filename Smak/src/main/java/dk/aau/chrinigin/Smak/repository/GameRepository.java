package dk.aau.chrinigin.Smak.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import dk.aau.chrinigin.Smak.model.Game;

public interface GameRepository extends JpaRepository<Game, Long> {
    
}
