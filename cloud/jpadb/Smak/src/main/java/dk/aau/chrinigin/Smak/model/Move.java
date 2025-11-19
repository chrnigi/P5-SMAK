package dk.aau.chrinigin.Smak.model;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;

@Entity
@IdClass(MoveId.class)
@Table(name = "moves")
public class Move {

    @Id
    private Long id;      // game id

    @Id
    private Integer plyno; // half-move number 

    // THE ONLY move encoding we keep
    private String uci;   // e.g. "e2e4", "e7e8q"

    @Enumerated(EnumType.STRING)
    private PieceType pieceMoved;     // what moved

    @Enumerated(EnumType.STRING)
    private PieceType pieceCaptured;  // what was captured (or NONE)

    // True if this move was a promotion (we only promote to queen)
    private Boolean promotion;

    // Optional: SAN string may come from Arduino just incase but can delete later
    @Transient
    private String san;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "id", insertable = false, updatable = false)
    @JsonIgnore
    @JsonIgnoreProperties({"hibernateLazyInitializer", "handler"})
    private Game game;

    public Move() {}

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Integer getPlyno() {
        return plyno;
    }

    public void setPlyno(Integer plyno) {
        this.plyno = plyno;
    }

    public String getUci() {
        return uci;
    }

    public void setUci(String uci) {
        this.uci = uci;
    }

    public PieceType getPieceMoved() {
        return pieceMoved;
    }

    public void setPieceMoved(PieceType pieceMoved) {
        this.pieceMoved = pieceMoved;
    }

    public PieceType getPieceCaptured() {
        return pieceCaptured;
    }

    public void setPieceCaptured(PieceType pieceCaptured) {
        this.pieceCaptured = pieceCaptured;
    }

    public Boolean getPromotion() {
        return promotion;
    }

    public void setPromotion(Boolean promotion) {
        this.promotion = promotion;
    }

    public String getSan() {
        return san;
    }

    public void setSan(String san) {
        this.san = san;
    }

    @JsonIgnore
    public Game getGame() {
        return game;
    }

    @JsonIgnore
    public void setGame(Game game) {
        this.game = game;
    }
}
