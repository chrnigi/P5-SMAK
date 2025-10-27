package dk.aau.chrinigin.Smak.model;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.FetchType;
import jakarta.persistence.Id;
import jakarta.persistence.IdClass;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;

@IdClass(BoardstateId.class)
@Table(name = "boardstates")
@Entity
public class Boardstate {
    @Id
    @Column(name = "id", nullable = false)
    private Long id;

    @Column(name = "x_placement")
    private Short x_placement;

    @Column(name = "o_placement")
    private Short o_placement;

    @Id
    @Column(name = "moveno")
    private Integer moveno;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "id")
    @JsonIgnoreProperties({"hibernateLazyInitializer", "handler"}) 
    private Game game;

    public Boardstate() {}

    public Boardstate(Long id, Short x_placement, Short o_placement, Integer moveno) {
        this.id = id;
        this.x_placement = x_placement;
        this.o_placement = o_placement;
        this.moveno = moveno;
        
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Short getX_placement() {
        return x_placement;
    }

    public void setX_placement(Short x_placement) {
        this.x_placement = x_placement;
    }

    public Short getO_placement() {
        return o_placement;
    }

    public void setO_placement(Short o_placement) {
        this.o_placement = o_placement;
    }

    public Integer getMoveno() {
        return moveno;
    }

    public void setMoveno(Integer moveno) {
        this.moveno = moveno;
    }

    public Game getGame() {
        return game;
    }

    public void setGame(Game game) {
        this.game = game;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((id == null) ? 0 : id.hashCode());
        result = prime * result + ((x_placement == null) ? 0 : x_placement.hashCode());
        result = prime * result + ((o_placement == null) ? 0 : o_placement.hashCode());
        result = prime * result + ((moveno == null) ? 0 : moveno.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        Boardstate other = (Boardstate) obj;
        if (id == null) {
            if (other.id != null)
                return false;
        } else if (!id.equals(other.id))
            return false;
        if (x_placement == null) {
            if (other.x_placement != null)
                return false;
        } else if (!x_placement.equals(other.x_placement))
            return false;
        if (o_placement == null) {
            if (other.o_placement != null)
                return false;
        } else if (!o_placement.equals(other.o_placement))
            return false;
        if (moveno == null) {
            if (other.moveno != null)
                return false;
        } else if (!moveno.equals(other.moveno))
            return false;
        return true;
    }

    @Override
    public String toString() {
        return "Boardstate [id=" + id + ", x_placement=" + x_placement + ", o_placement=" + o_placement + ", moveno="
                + moveno + ", game=" + game + "]";
    }

    
}

