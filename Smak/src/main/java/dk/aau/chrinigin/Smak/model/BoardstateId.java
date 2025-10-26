package dk.aau.chrinigin.Smak.model;

import java.io.Serializable;
import java.util.Objects;

public class BoardstateId implements Serializable {
    private Long id;
    private Integer moveno;

    public BoardstateId() {}

    public BoardstateId(Long id, Integer moveno) {
        this.id = id;
        this.moveno = moveno;
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.id, this.moveno);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (!(o instanceof BoardstateId)) {
            return false;
        }
        BoardstateId b = (BoardstateId) o;
        return Objects.equals(this.id, b.id) && Objects.equals(this.moveno, b.moveno);
    }
}