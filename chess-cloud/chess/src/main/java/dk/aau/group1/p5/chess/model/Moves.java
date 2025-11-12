package dk.aau.group1.p5.chess.model;


import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Table;

@Entity
@Table(name = "moves")
public class Moves {
    
    @Column(name = "move_number", nullable = false)
    private Integer move_number;

    @Column(name = "ply_number", nullable = false)
    private Integer ply_number;

    @Column(name = "is_capture", nullable = false)
    private Boolean is_capture = false;

    @Column(name = "is_castles_kingside", nullable = false)
    private Boolean is_castles_kingside = false;

    @Column(name = "is_castles_queenside", nullable = false)
    private Boolean is_castles_queenside = false;

    @Column(name = "piece_moved", nullable = false)
    private PiecesEnum piece_moved;

    @Column(name = "piece_captured", nullable = true)
    private PiecesEnum piece_captured;

    @Column(name = "from_square", nullable = false)
    private Short from_square;

    @Column(name = "to_square", nullable = false)
    private Short to_square;

}
