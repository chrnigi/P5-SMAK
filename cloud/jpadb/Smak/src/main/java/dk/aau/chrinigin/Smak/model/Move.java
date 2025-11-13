package dk.aau.chrinigin.Smak.model;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import java.sql.Timestamp;

// Represents a single chess move in a game. Each move stores its ply number,
// SAN/UCI notation, the resulting FEN, and flags such as check, mate,
// captures, and promotions. Uses (game id + plyno) as a composite key.

@Entity
@IdClass(MoveId.class)
@Table(name = "moves")
public class Move {

    @Id
    private Long id;            

    @Id
    private Integer plyno;      

    private String san;         
    private String uci;         

    @Column(length = 128)
    private String fenAfter;   

    private Boolean isCheck;
    private Boolean isMate;
    private String captured;    
    private String promotion;   
    private Timestamp movedAt;

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

    public String getSan() { return san; }
    public void setSan(String san) { this.san = san; }

    public String getUci() { return uci; }
    public void setUci(String uci) { this.uci = uci; }

    public String getFenAfter() { return fenAfter; }
    public void setFenAfter(String fenAfter) { this.fenAfter = fenAfter; }

    public Boolean getIsCheck() { return isCheck; }
    public void setIsCheck(Boolean isCheck) { this.isCheck = isCheck; }

    public Boolean getIsMate() { return isMate; }
    public void setIsMate(Boolean isMate) { this.isMate = isMate; }

    public String getCaptured() { return captured; }
    public void setCaptured(String captured) { this.captured = captured; }

    public String getPromotion() { return promotion; }
    public void setPromotion(String promotion) { this.promotion = promotion; }

    public Timestamp getMovedAt() { return movedAt; }
    public void setMovedAt(Timestamp movedAt) { this.movedAt = movedAt; }

    @JsonIgnore
    public Game getGame() { return game; }
    @JsonIgnore
    public void setGame(Game game) { this.game = game; }
}
