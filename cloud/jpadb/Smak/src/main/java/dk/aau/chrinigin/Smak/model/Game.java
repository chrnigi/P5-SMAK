package dk.aau.chrinigin.Smak.model;

import java.sql.Timestamp;
import java.util.Objects;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;


@Entity
@Table(name = "games")
public class Game {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id", nullable = false)
    private Long id;

    @Column(name = "result", nullable = false)
    @Enumerated(EnumType.STRING)
    ResultEnum result;

    @Column(name = "gamestart")
    Timestamp gamestart;

    public Game() {}

    public Game(ResultEnum result, Timestamp gamestart) {
        this.result = result;
        this.gamestart = gamestart;
    }

    @Override
    public String toString() {
        return "Game [id=" + id + ", result=" + result + ", gamestart=" + gamestart + "]";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        
        if (!(o instanceof Game))
            return false;

        Game game = (Game) o;

        return 
        Objects.equals(this.id, game.id) && 
        Objects.equals(this.result, game.result) && 
        Objects.equals(this.gamestart, game.gamestart);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(this.id, this.result, this.gamestart);
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public ResultEnum getResult() {
        return result;
    }

    public void setResult(ResultEnum result) {
        this.result = result;
    }

    public Timestamp getGamestart() {
        return gamestart;
    }

    public void setGamestart(Timestamp gamestart) {
        this.gamestart = gamestart;
    }

}
