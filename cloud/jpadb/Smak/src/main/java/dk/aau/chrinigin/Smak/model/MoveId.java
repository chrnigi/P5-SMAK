package dk.aau.chrinigin.Smak.model;

import java.io.Serializable;
import java.util.Objects;

// Composite key for Move: uniquely identifies each move by combining
// the game id and the ply number. Required for JPA to map Move correctly.

public class MoveId implements Serializable {
    private Long id;       
    private Integer plyno; 

    public MoveId() {}

    public MoveId(Long id, Integer plyno) {
        this.id = id;
        this.plyno = plyno;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof MoveId)) return false;
        MoveId that = (MoveId) o;
        return Objects.equals(id, that.id) && Objects.equals(plyno, that.plyno);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, plyno);
    }
}
