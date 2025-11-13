package dk.aau.chrinigin.Smak.model;

import jakarta.persistence.*;
import java.sql.Timestamp;
// Database entity representing a chess game: stores start/end time, result,
// initial board (FEN), latest position, and the running PGN for the game.
@Entity
@Table(name = "games")
public class Game {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Enumerated(EnumType.STRING)
    private ResultEnum result = ResultEnum.INPROGRESS;

    private Timestamp gamestart;

    // NEW for chess
    @Column(length = 128)
    private String initialFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    private Timestamp gameend;

    private String latestFen;    
    @Lob
    private String pgn;           

    public Game() {}

    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }

    public ResultEnum getResult() { return result; }
    public void setResult(ResultEnum result) { this.result = result; }

    public Timestamp getGamestart() { return gamestart; }
    public void setGamestart(Timestamp gamestart) { this.gamestart = gamestart; }

    public String getInitialFen() { return initialFen; }
    public void setInitialFen(String initialFen) { this.initialFen = initialFen; }

    public Timestamp getGameend() { return gameend; }
    public void setGameend(Timestamp gameend) { this.gameend = gameend; }

    public String getLatestFen() { return latestFen; }
    public void setLatestFen(String latestFen) { this.latestFen = latestFen; }

    public String getPgn() { return pgn; }
    public void setPgn(String pgn) { this.pgn = pgn; }
}
