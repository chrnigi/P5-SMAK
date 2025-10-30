package dk.aau.chrinigin.Smak.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.ResultEnum;

public interface GameRepository extends JpaRepository<Game, Long> {

}
