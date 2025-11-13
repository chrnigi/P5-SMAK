package dk.aau.chrinigin.Smak.repository;
// JPA repository for managing Game entities. Provides CRUD operations

import org.springframework.data.jpa.repository.JpaRepository;
import dk.aau.chrinigin.Smak.model.Game;

public interface GameRepository extends JpaRepository<Game, Long> {

}
