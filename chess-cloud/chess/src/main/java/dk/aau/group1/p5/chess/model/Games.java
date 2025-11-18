package dk.aau.group1.p5.chess.model;

import java.sql.Timestamp;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

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
@JsonIgnoreProperties(ignoreUnknown = true, value = {"id"}, allowGetters = true) 
public class Games {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id", nullable = false)
    private Long id;

    @Column(name = "gamestate", nullable = false)
    @Enumerated(EnumType.STRING)
    private GameStateEnum gamestate;

    @Column(name = "gamestart")
    private Timestamp gamestart;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public GameStateEnum getGamestate() {
        return gamestate;
    }

    public void setGamestate(GameStateEnum gamestate) {
        this.gamestate = gamestate;
    }

    public Timestamp getGamestart() {
        return gamestart;
    }

    public void setGamestart(Timestamp gamestart) {
        this.gamestart = gamestart;
    }

    public Games() {
    }

    public Games(Long id, GameStateEnum gamestate, Timestamp gamestart) {
        this.id = id;
        this.gamestate = gamestate;
        this.gamestart = gamestart;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((id == null) ? 0 : id.hashCode());
        result = prime * result + ((gamestate == null) ? 0 : gamestate.hashCode());
        result = prime * result + ((gamestart == null) ? 0 : gamestart.hashCode());
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
        Games other = (Games) obj;
        if (id == null) {
            if (other.id != null)
                return false;
        } else if (!id.equals(other.id))
            return false;
        if (gamestate != other.gamestate)
            return false;
        if (gamestart == null) {
            if (other.gamestart != null)
                return false;
        } else if (!gamestart.equals(other.gamestart))
            return false;
        return true;
    }

    @Override
    public String toString() {
        return "Games [id=" + id + ", gamestate=" + gamestate + ", gamestart=" + gamestart + "]";
    }

    

}
