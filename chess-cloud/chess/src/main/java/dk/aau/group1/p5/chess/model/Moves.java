package dk.aau.group1.p5.chess.model;


import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.FetchType;
import jakarta.persistence.Id;
import jakarta.persistence.IdClass;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;

@Entity
@IdClass(MoveId.class)
@Table(name = "moves")
public class Moves {
    @Id
    @Column(name = "id", nullable = false)
    Long id;

    @Column(name = "move_number", nullable = false)
    private Integer move_number;

    @Id
    @Column(name = "ply_number", nullable = false)
    private Integer ply_number;

    @Column(name = "piece_moved", nullable = false)
    private PiecesEnum piece_moved;

    @Column(name = "piece_captured", nullable = true)
    private PiecesEnum piece_captured;

    @Column(name = "is_promotion", nullable = false)
    private Boolean is_promotion = false;

    @Column(name = "is_capture", nullable = false)
    private Boolean is_capture = false;

    @Column(name = "is_enpassant", nullable = false)
    private Boolean is_enpassant = false;

    @Column(name = "is_castles", nullable = false)
    private Boolean is_castles = false;

    @Column(name = "from_square", nullable = false)
    private Short from_square;

    @Column(name = "to_square", nullable = false)
    private Short to_square;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "id", insertable = false, updatable = false)
    @JsonIgnoreProperties({"hibernateLazyInitializer", "handler"})
    private Games game;

    public Moves() {}

    public Moves(Long id, Integer move_number, Integer ply_number, PiecesEnum piece_moved, PiecesEnum piece_captured,
            Boolean is_promotion, Boolean is_capture, Boolean is_enpassant, Boolean is_castles, Short from_square,
            Short to_square) {
        this.id = id;
        this.move_number = move_number;
        this.ply_number = ply_number;
        this.piece_moved = piece_moved;
        this.piece_captured = piece_captured;
        this.is_promotion = is_promotion;
        this.is_capture = is_capture;
        this.is_enpassant = is_enpassant;
        this.is_castles = is_castles;
        this.from_square = from_square;
        this.to_square = to_square;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Integer getMove_number() {
        return move_number;
    }

    public void setMove_number(Integer move_number) {
        this.move_number = move_number;
    }

    public Integer getPly_number() {
        return ply_number;
    }

    public void setPly_number(Integer ply_number) {
        this.ply_number = ply_number;
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

    public Boolean getIs_promotion() {
        return is_promotion;
    }

    public void setIs_promotion(Boolean is_promotion) {
        this.is_promotion = is_promotion;
    }

    public Boolean getIs_capture() {
        return is_capture;
    }

    public void setIs_capture(Boolean is_capture) {
        this.is_capture = is_capture;
    }

    public Boolean getIs_enpassant() {
        return is_enpassant;
    }

    public void setIs_enpassant(Boolean is_enpassant) {
        this.is_enpassant = is_enpassant;
    }

    public Boolean getIs_castles() {
        return is_castles;
    }

    public void setIs_castles(Boolean is_castles) {
        this.is_castles = is_castles;
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

    public Games getGame() {
        return game;
    }

    public void setGame(Games game) {
        this.game = game;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((id == null) ? 0 : id.hashCode());
        result = prime * result + ((move_number == null) ? 0 : move_number.hashCode());
        result = prime * result + ((ply_number == null) ? 0 : ply_number.hashCode());
        result = prime * result + ((piece_moved == null) ? 0 : piece_moved.hashCode());
        result = prime * result + ((piece_captured == null) ? 0 : piece_captured.hashCode());
        result = prime * result + ((is_promotion == null) ? 0 : is_promotion.hashCode());
        result = prime * result + ((is_capture == null) ? 0 : is_capture.hashCode());
        result = prime * result + ((is_enpassant == null) ? 0 : is_enpassant.hashCode());
        result = prime * result + ((is_castles == null) ? 0 : is_castles.hashCode());
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
        Moves other = (Moves) obj;
        if (id == null) {
            if (other.id != null)
                return false;
        } else if (!id.equals(other.id))
            return false;
        if (move_number == null) {
            if (other.move_number != null)
                return false;
        } else if (!move_number.equals(other.move_number))
            return false;
        if (ply_number == null) {
            if (other.ply_number != null)
                return false;
        } else if (!ply_number.equals(other.ply_number))
            return false;
        if (piece_moved != other.piece_moved)
            return false;
        if (piece_captured != other.piece_captured)
            return false;
        if (is_promotion == null) {
            if (other.is_promotion != null)
                return false;
        } else if (!is_promotion.equals(other.is_promotion))
            return false;
        if (is_capture == null) {
            if (other.is_capture != null)
                return false;
        } else if (!is_capture.equals(other.is_capture))
            return false;
        if (is_enpassant == null) {
            if (other.is_enpassant != null)
                return false;
        } else if (!is_enpassant.equals(other.is_enpassant))
            return false;
        if (is_castles == null) {
            if (other.is_castles != null)
                return false;
        } else if (!is_castles.equals(other.is_castles))
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
        return "Moves [id=" + id + ", move_number=" + move_number + ", ply_number=" + ply_number + ", piece_moved="
                + piece_moved + ", piece_captured=" + piece_captured + ", is_promotion=" + is_promotion
                + ", is_capture=" + is_capture + ", is_enpassant=" + is_enpassant + ", is_castles=" + is_castles
                + ", from_square=" + from_square + ", to_square=" + to_square + "]";
    }

        

}
