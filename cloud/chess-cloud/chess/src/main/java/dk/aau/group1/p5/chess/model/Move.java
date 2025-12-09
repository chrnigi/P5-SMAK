package dk.aau.group1.p5.chess.model;


import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.FetchType;
import jakarta.persistence.Id;
import jakarta.persistence.IdClass;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;

@Entity
@IdClass(MoveId.class)
@Table(name = "moves")
public class Move {
    @Id
    @Column(name = "id", nullable = false)
    private Long id;

    @Id
    @Column(name = "ply_number", nullable = false)
    private Integer ply_number;

    @Column(name = "move_type", nullable = false)
    @Enumerated(EnumType.STRING)
    private MoveTypeEnum move_type;

    @Column(name = "piece_moved", nullable = false)
    @Enumerated(EnumType.STRING)
    private PiecesEnum piece_moved;

    @Column(name = "piece_captured", nullable = false)
    @Enumerated(EnumType.STRING)
    private PiecesEnum piece_captured;

    @Column(name = "from_square", nullable = false)
    private Short from_square;

    @Column(name = "to_square", nullable = false)
    private Short to_square;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "id", insertable = false, updatable = false)
    @JsonIgnoreProperties({"hibernateLazyInitializer", "handler"})
    private Game game;

    public Move() {}

    public Move(Long id, Integer ply_number, MoveTypeEnum move_type, PiecesEnum piece_moved, PiecesEnum piece_captured,
            Short from_square, Short to_square) {
        this.id = id;
        this.ply_number = ply_number;
        this.move_type = move_type;
        this.piece_moved = piece_moved;
        this.piece_captured = piece_captured;
        this.from_square = from_square;
        this.to_square = to_square;
    }



    
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((id == null) ? 0 : id.hashCode());
        result = prime * result + ((ply_number == null) ? 0 : ply_number.hashCode());
        result = prime * result + ((move_type == null) ? 0 : move_type.hashCode());
        result = prime * result + ((piece_moved == null) ? 0 : piece_moved.hashCode());
        result = prime * result + ((piece_captured == null) ? 0 : piece_captured.hashCode());
        result = prime * result + ((from_square == null) ? 0 : from_square.hashCode());
        result = prime * result + ((to_square == null) ? 0 : to_square.hashCode());
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
        Move other = (Move) obj;
        if (id == null) {
            if (other.id != null)
                return false;
        } else if (!id.equals(other.id))
            return false;
        if (ply_number == null) {
            if (other.ply_number != null)
                return false;
        } else if (!ply_number.equals(other.ply_number))
            return false;
        if (move_type != other.move_type)
            return false;
        if (piece_moved != other.piece_moved)
            return false;
        if (piece_captured != other.piece_captured)
            return false;
        if (from_square == null) {
            if (other.from_square != null)
                return false;
        } else if (!from_square.equals(other.from_square))
            return false;
        if (to_square == null) {
            if (other.to_square != null)
                return false;
        } else if (!to_square.equals(other.to_square))
            return false;
        return true;
    }

    

    @Override
    public String toString() {
        return "Move [id=" + id + ", ply_number=" + ply_number + ", move_type=" + move_type + ", piece_moved="
                + piece_moved + ", piece_captured=" + piece_captured + ", from_square=" + from_square + ", to_square="
                + to_square + "]";
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Integer getPly_number() {
        return ply_number;
    }

    public void setPly_number(Integer ply_number) {
        this.ply_number = ply_number;
    }

    public MoveTypeEnum getMove_type() {
        return move_type;
    }

    public void setMove_type(MoveTypeEnum move_type) {
        this.move_type = move_type;
    }

    public PiecesEnum getPiece_moved() {
        return piece_moved;
    }

    public void setPiece_moved(PiecesEnum piece_moved) {
        this.piece_moved = piece_moved;
    }

    public PiecesEnum getPiece_captured() {
        return piece_captured;
    }

    public void setPiece_captured(PiecesEnum piece_captured) {
        this.piece_captured = piece_captured;
    }

    public Short getFrom_square() {
        return from_square;
    }

    public void setFrom_square(Short from_square) {
        this.from_square = from_square;
    }

    public Short getTo_square() {
        return to_square;
    }

    public void setTo_square(Short to_square) {
        this.to_square = to_square;
    }

    // public Game getGame() {
    //     return game;
    // }

    // public void setGame(Game game) {
    //     this.game = game;
    // }


}
