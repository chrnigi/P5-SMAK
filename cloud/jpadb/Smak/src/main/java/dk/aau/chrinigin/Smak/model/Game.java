package dk.aau.chrinigin.Smak.model;

import jakarta.persistence.*;
import java.sql.Timestamp;

/**
 * Represents a single chess game in the database.
 * We only store:
 *  - id           : internal game id
 *  - gamestart    : when this game was started
 *  - gameend      : when it finished (or null if still running)
 *  - state        : PRE_GAME, WHITE_TO_MOVE, BLACK_TO_MOVE, WHITE_WIN, BLACK_WIN, DRAW, ILLEGAL_STATE
 */
@Entity
@Table(name = "games")
public class Game {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private Timestamp gamestart;

    private Timestamp gameend;

    @Enumerated(EnumType.STRING)
    private GameState state = GameState.PRE_GAME;

    public Game() {}

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Timestamp getGamestart() {
        return gamestart;
    }

    public void setGamestart(Timestamp gamestart) {
        this.gamestart = gamestart;
    }

    public Timestamp getGameend() {
        return gameend;
    }

    public void setGameend(Timestamp gameend) {
        this.gameend = gameend;
    }

    public GameState getState() {
        return state;
    }

    public void setState(GameState state) {
        this.state = state;
    }
}
