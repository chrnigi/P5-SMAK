package dk.aau.group1.p5.chess.model;

import java.io.Serializable;
import java.util.Objects;

public class MoveId implements Serializable {
    
    private Long id;
    private Integer ply_number;
    
    

    public Long getId() {
        return id;
    }
    public void setId(Long id) {
        this.id = id;
    }
    public Integer getPly_number() {
        return ply_number;
    }
    public void setPly_number(Integer moveno) {
        this.ply_number = moveno;
    }

    @Override
    public String toString() {
        return "MoveId [id=" + id + ", moveno=" + ply_number + "]";
    }
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((id == null) ? 0 : id.hashCode());
        result = prime * result + ((ply_number == null) ? 0 : ply_number.hashCode());
        return result;
    }
    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (!(o instanceof MoveId))
            return false;
        MoveId m = (MoveId) o;

        return Objects.equals(this.id, m.id) && Objects.equals(this.ply_number, m.ply_number);  
    }
    
}
