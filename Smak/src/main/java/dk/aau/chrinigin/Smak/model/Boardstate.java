package dk.aau.chrinigin.Smak.model;

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
    private Game game;

    public Boardstate() {}

    public Boardstate(Long id, Short x_placement, Short o_placement, Integer moveno, Game game) {
        this.id = id;
        this.x_placement = x_placement;
        this.o_placement = o_placement;
        this.moveno = moveno;
        this.game = game;
    }
}

