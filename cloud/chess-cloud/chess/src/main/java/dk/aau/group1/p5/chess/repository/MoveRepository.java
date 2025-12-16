package dk.aau.group1.p5.chess.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import dk.aau.group1.p5.chess.model.MoveId;
import dk.aau.group1.p5.chess.model.Game;
import dk.aau.group1.p5.chess.model.Move;

public interface MoveRepository extends JpaRepository<Move, MoveId> {
    
    @Query(
        value = "SELECT * FROM moves m WHERE m.id = ?1",
        nativeQuery = true
    )
    public List<Move> getByGameId(Long id);

    @Query(
        value = "SELECT * FROM games g WHERE g.id = ?1",
        nativeQuery = true
    )
    public Optional<Game> checkGameId(Long id);
}
