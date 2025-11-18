package dk.aau.chrinigin.Smak.web;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.model.PieceType;
import dk.aau.chrinigin.Smak.repository.GameRepository;
import dk.aau.chrinigin.Smak.service.MoveService;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import com.github.bhlangonijr.chesslib.Board;
import com.github.bhlangonijr.chesslib.Piece;
import com.github.bhlangonijr.chesslib.Square;
import com.github.bhlangonijr.chesslib.Side;

import java.util.Comparator;
import java.util.List;

@RestController
public class MoveController {

    private final MoveService moveService;
    private final GameRepository gameRepository;

    public MoveController(MoveService moveService, GameRepository gameRepository) {
        this.moveService = moveService;
        this.gameRepository = gameRepository;
    }

    @GetMapping("/moves/ping")
    public String movesPing() {
        return "MoveController is alive";
    }

    @GetMapping("/moves")
    List<Move> all() {
        return moveService.findAll();
    }

    @GetMapping("/moves/{id}")
    List<Move> byGame(@PathVariable Long id) {
        return moveService.getByGameId(id);
    }

    @PostMapping("/moves")
    Move newMove(@RequestBody Move move) {

        if (move.getId() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Move must contain game id");
        }
        if (move.getPlyno() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Move must contain plyno");
        }

        String uci = move.getUci();
        if (uci == null || (uci.length() != 4 && uci.length() != 5)) {
            throw new ResponseStatusException(
                    HttpStatus.BAD_REQUEST,
                    "UCI must be 4 or 5 characters, e.g. e2e4 or a7a8q"
            );
        }

        Game game = gameRepository.findById(move.getId())
                .orElseThrow(() -> new ResponseStatusException(
                        HttpStatus.NOT_FOUND, "Game not found: " + move.getId()
                ));

        // 1) rebuild board from start using all existing moves
        Board board = new Board(); // standard starting position

        List<Move> existing = moveService.getByGameId(move.getId());
        existing.sort(Comparator.comparing(Move::getPlyno));

        for (Move m : existing) {
            applyUci(board, m.getUci());
        }

        // 2) compute piece moved / captured BEFORE applying the new move
        Square fromSq = Square.fromValue(uci.substring(0, 2).toUpperCase());
        Square toSq   = Square.fromValue(uci.substring(2, 4).toUpperCase());
        

        Piece fromPiece = board.getPiece(fromSq);
        Piece toPieceBefore = board.getPiece(toSq);

        // piece that moves (color-specific)
        PieceType movedType = mapPieceType(fromPiece);
        move.setPieceMoved(movedType);

        // promotion (we only care about promotion to queen via 'q')
        boolean isPromotion = (uci.length() == 5 && Character.toLowerCase(uci.charAt(4)) == 'q');
        move.setPromotion(isPromotion);

        // piece captured (if any)
        PieceType capturedType = null;

        if (toPieceBefore != Piece.NONE) {
            // if target has same-color piece → illegal move
            if (sameColor(fromPiece, toPieceBefore)) {
            throw new ResponseStatusException(
                HttpStatus.BAD_REQUEST,
                "Illegal move: cannot capture own piece on " + uci.substring(2, 4)
            );
            }
            // normal capture of opponent piece
            capturedType = mapPieceType(toPieceBefore);
        } else {
            // possible en passant: pawn moves diagonally to an empty square
            if (fromPiece == Piece.WHITE_PAWN || fromPiece == Piece.BLACK_PAWN) {
            int fileDiff = Math.abs(uci.charAt(0) - uci.charAt(2));
            int rankDiff = Math.abs(uci.charAt(1) - uci.charAt(3));
            if (fileDiff == 1 && rankDiff == 1) {
                capturedType = (fromPiece == Piece.WHITE_PAWN)
                    ? PieceType.B_PAWN
                    : PieceType.W_PAWN;
            }
            }
        }

        move.setPieceCaptured(capturedType);


        // 3) actually apply the move to validate legality
        boolean ok = applyUci(board, uci);
        if (!ok) {
            throw new ResponseStatusException(
                    HttpStatus.BAD_REQUEST,
                    "Illegal move for current position: " + uci
            );
        }

        // 4) save
        return moveService.save(move);
    }

    // apply a UCI move on a chesslib Board
    private static boolean applyUci(Board board, String uci) {
        if (uci == null || (uci.length() != 4 && uci.length() != 5)) {
            return false;
        }

        Square from = Square.fromValue(uci.substring(0, 2).toUpperCase());
        Square to   = Square.fromValue(uci.substring(2, 4).toUpperCase());

        com.github.bhlangonijr.chesslib.move.Move chessMove;

        if (uci.length() == 5) {
            char promChar = Character.toLowerCase(uci.charAt(4));
            Side side = board.getSideToMove();
            Piece promoPiece = switch (promChar) {
                case 'q' -> (side == Side.WHITE ? Piece.WHITE_QUEEN  : Piece.BLACK_QUEEN);
                case 'r' -> (side == Side.WHITE ? Piece.WHITE_ROOK   : Piece.BLACK_ROOK);
                case 'b' -> (side == Side.WHITE ? Piece.WHITE_BISHOP : Piece.BLACK_BISHOP);
                case 'n' -> (side == Side.WHITE ? Piece.WHITE_KNIGHT : Piece.BLACK_KNIGHT);
                default  -> null;
            };
            if (promoPiece == null) return false;
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to, promoPiece);
        } else {
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to);
        }

        return board.doMove(chessMove);
    }

    // map chesslib Piece → our color-specific PieceType
    private static PieceType mapPieceType(Piece p) {
        if (p == null) return PieceType.NONE;

        return switch (p) {
            case WHITE_PAWN  -> PieceType.W_PAWN;
            case BLACK_PAWN  -> PieceType.B_PAWN;
            case WHITE_KNIGHT-> PieceType.W_KNIGHT;
            case BLACK_KNIGHT-> PieceType.B_KNIGHT;
            case WHITE_BISHOP-> PieceType.W_BISHOP;
            case BLACK_BISHOP-> PieceType.B_BISHOP;
            case WHITE_ROOK  -> PieceType.W_ROOK;
            case BLACK_ROOK  -> PieceType.B_ROOK;
            case WHITE_QUEEN -> PieceType.W_QUEEN;
            case BLACK_QUEEN -> PieceType.B_QUEEN;
            case WHITE_KING  -> PieceType.W_KING;
            case BLACK_KING  -> PieceType.B_KING;
            default          -> PieceType.NONE;
        };
    }

    private static boolean sameColor(Piece a, Piece b) {
        if (a == null || b == null || a == Piece.NONE || b == Piece.NONE) {
            return false;
        }
        return isWhite(a) == isWhite(b);
    }

    private static boolean isWhite(Piece p) {
        if (p == null) return false;
        return switch (p) {
            case WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP,
                 WHITE_ROOK, WHITE_QUEEN, WHITE_KING -> true;
            default -> false;
        };
    }

}
