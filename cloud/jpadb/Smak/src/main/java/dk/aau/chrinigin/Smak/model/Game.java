package dk.aau.chrinigin.Smak.model;

import jakarta.persistence.*;
import java.sql.Timestamp;

@Entity
@Table(name = "games")
public class Game {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;          // game id

    // when the game started
    private Timestamp gamestart;

    // when the game ended (null if still ongoing)
    private Timestamp gameend;

    @Enumerated(EnumType.STRING)
    private GameState state = GameState.PRE_GAME;

    public Game() {}

    public Long getId() { return id; }

    public Timestamp getGamestart() { return gamestart; }
    public void setGamestart(Timestamp gamestart) { this.gamestart = gamestart; }

    public Timestamp getGameend() { return gameend; }
    public void setGameend(Timestamp gameend) { this.gameend = gameend; }

    public GameState getState() { return state; }
    public void setState(GameState state) { this.state = state; }
}
