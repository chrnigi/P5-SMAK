package dk.aau.group1.p5.chess.model;


import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.IdClass;
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

}
