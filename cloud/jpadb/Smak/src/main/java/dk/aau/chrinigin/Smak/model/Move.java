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
    private Integer plyno; // half-move number in the game

    // The only move encoding we store in the DB
    private String uci;

    // match DB column: piece_type_moved (enum stored as STRING)
    @Enumerated(EnumType.STRING)
    @Column(name = "piece_type_moved")
    private PieceType pieceMoved;

    // match DB column: piece_type_captured (enum stored as STRING)
    @Enumerated(EnumType.STRING)
    @Column(name = "piece_type_captured")
    private PieceType pieceCaptured;

    // promotion is a Boolean; DB column 'promotion' can be TINYINT(1) or BOOLEAN
    private Boolean promotion;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "id", insertable = false, updatable = false)
    @JsonIgnore
    @JsonIgnoreProperties({"hibernateLazyInitializer","handler"})
    private Game game;

    public Move() {}

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }

    public Integer getPlyno() { return plyno; }
    public void setPlyno(Integer plyno) { this.plyno = plyno; }

    public String getUci() { return uci; }
    public void setUci(String uci) { this.uci = uci; }

    public PieceType getPieceMoved() { return pieceMoved; }
    public void setPieceMoved(PieceType pieceMoved) { this.pieceMoved = pieceMoved; }

    public PieceType getPieceCaptured() { return pieceCaptured; }
    public void setPieceCaptured(PieceType pieceCaptured) { this.pieceCaptured = pieceCaptured; }

    public Boolean getPromotion() { return promotion; }
    public void setPromotion(Boolean promotion) { this.promotion = promotion; }

    @JsonIgnore
    public Game getGame() { return game; }

    @JsonIgnore
    public void setGame(Game game) { this.game = game; }
}
