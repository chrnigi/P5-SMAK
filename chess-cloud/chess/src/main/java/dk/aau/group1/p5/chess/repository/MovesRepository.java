package dk.aau.group1.p5.chess.repository;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import dk.aau.group1.p5.chess.model.MoveId;
import dk.aau.group1.p5.chess.model.Moves;

public interface MovesRepository extends JpaRepository<Moves, MoveId> {
    
    @Query(
        value = "SELECT * FROM moves m WHERE m.id = ?1",
        nativeQuery = true
    )
    public List<Moves> getByGameId(Long id);

}
