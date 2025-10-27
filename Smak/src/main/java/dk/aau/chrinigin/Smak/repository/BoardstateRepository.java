package dk.aau.chrinigin.Smak.repository;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import dk.aau.chrinigin.Smak.model.Boardstate;
import dk.aau.chrinigin.Smak.model.BoardstateId;


public interface BoardstateRepository extends JpaRepository<Boardstate, BoardstateId> {
   
    @Query(
        value = "SELECT * FROM boardstates b WHERE b.id = ?1",
        nativeQuery = true
    )
    public List<Boardstate> getByGameId(Long id);

    
}
