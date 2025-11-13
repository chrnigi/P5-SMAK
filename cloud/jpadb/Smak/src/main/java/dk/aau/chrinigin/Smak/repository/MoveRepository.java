package dk.aau.chrinigin.Smak.repository;
// JPA repository for managing Move entities. Provides CRUD operations
// and a custom query to retrieve moves by game ID ordered by ply number.

import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.model.MoveId;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import java.util.List;

public interface MoveRepository extends JpaRepository<Move, MoveId> {

    @Query(value = "SELECT * FROM moves m WHERE m.id = ?1 ORDER BY m.plyno ASC", nativeQuery = true)
    List<Move> getByGameId(Long id);
}
