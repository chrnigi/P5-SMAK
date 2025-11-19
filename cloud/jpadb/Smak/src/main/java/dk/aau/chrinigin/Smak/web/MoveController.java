package dk.aau.chrinigin.Smak.web;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.GameState;
import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.model.PieceType;
import dk.aau.chrinigin.Smak.repository.GameRepository;
import dk.aau.chrinigin.Smak.service.MoveService;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import com.github.bhlangonijr.chesslib.Board;
import com.github.bhlangonijr.chesslib.Square;
import com.github.bhlangonijr.chesslib.Side;
import com.github.bhlangonijr.chesslib.Piece;

import java.sql.Timestamp;
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

    // Tiny health check
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

    /**
     * POST /moves
     * Body example (PowerShell):
     *  { "id": 4, "plyno": 1, "uci": "e2e4" }
     *
     * We:
     *  - validate input
     *  - rebuild board from all previous moves (UCI only)
     *  - detect pieceMoved / pieceCaptured for the new move
     *  - apply the move
     *  - update Game.state + gameend if the game finished
     *  - store move in DB
     */
    @PostMapping("/moves")
    Move newMove(@RequestBody Move move) {

        if (move.getId() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Move must contain game id");
        }
        if (move.getPlyno() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Move must contain plyno");
        }
        if (move.getUci() == null || move.getUci().length() < 4) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "UCI must be at least 4 chars, e.g. e2e4");
        }

        Game game = gameRepository.findById(move.getId())
                .orElseThrow(() -> new ResponseStatusException(
                        HttpStatus.NOT_FOUND, "Game not found: " + move.getId()));

        // 1) Load all existing moves, sort by ply
        List<Move> existing = moveService.getByGameId(move.getId());
        existing.sort(Comparator.comparingInt(m -> m.getPlyno() == null ? 0 : m.getPlyno()));

        // 2) Rebuild board from scratch (standard starting position)
        Board board = new Board();
        for (Move m : existing) {
            applyUciToBoard(board, m.getUci());
        }

        // 3) Detect pieceMoved / pieceCaptured for THIS move
        String uci = move.getUci();
        Square from = Square.fromValue(uci.substring(0, 2).toUpperCase());
        Square to   = Square.fromValue(uci.substring(2, 4).toUpperCase());

        Piece fromPiece = board.getPiece(from);
        Piece toPiece   = board.getPiece(to);

        move.setPieceMoved(mapPiece(fromPiece));
        move.setPieceCaptured(mapPiece(toPiece));

        boolean isPromotion = (uci.length() >= 5 &&
                (uci.charAt(4) == 'q' || uci.charAt(4) == 'Q'));
        move.setPromotion(isPromotion);

        // 4) Build and apply chesslib Move
        com.github.bhlangonijr.chesslib.move.Move chessMove;
        if (isPromotion) {
            Side side = board.getSideToMove();
            Piece promoPiece = (side == Side.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to, promoPiece);
        } else {
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to);
        }

        boolean ok = board.doMove(chessMove);
        if (!ok) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Illegal move for current position: " + uci);
        }

        // 5) Update game state / gameend
        updateGameStateFromBoard(game, board);

        gameRepository.save(game);
        return moveService.save(move);
    }

    /* ----- helpers below ----- */

    private void applyUciToBoard(Board board, String uci) {
        if (uci == null || uci.length() < 4) return;

        Square from = Square.fromValue(uci.substring(0, 2).toUpperCase());
        Square to   = Square.fromValue(uci.substring(2, 4).toUpperCase());

        com.github.bhlangonijr.chesslib.move.Move chessMove;

        if (uci.length() >= 5) {
            char prom = Character.toLowerCase(uci.charAt(4));
            Side side = board.getSideToMove();
            Piece promoPiece = switch (prom) {
                case 'q' -> (side == Side.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
                case 'r' -> (side == Side.WHITE ? Piece.WHITE_ROOK  : Piece.BLACK_ROOK);
                case 'b' -> (side == Side.WHITE ? Piece.WHITE_BISHOP: Piece.BLACK_BISHOP);
                case 'n' -> (side == Side.WHITE ? Piece.WHITE_KNIGHT: Piece.BLACK_KNIGHT);
                default  -> (side == Side.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
            };
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to, promoPiece);
        } else {
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to);
        }

        board.doMove(chessMove); // previous moves are assumed valid
    }

    private PieceType mapPiece(Piece p) {
        if (p == null || p == Piece.NONE) return PieceType.NONE;
        return switch (p) {
            case WHITE_PAWN   -> PieceType.W_PAWN;
            case WHITE_KNIGHT -> PieceType.W_KNIGHT;
            case WHITE_BISHOP -> PieceType.W_BISHOP;
            case WHITE_ROOK   -> PieceType.W_ROOK;
            case WHITE_QUEEN  -> PieceType.W_QUEEN;
            case WHITE_KING   -> PieceType.W_KING;
            case BLACK_PAWN   -> PieceType.B_PAWN;
            case BLACK_KNIGHT -> PieceType.B_KNIGHT;
            case BLACK_BISHOP -> PieceType.B_BISHOP;
            case BLACK_ROOK   -> PieceType.B_ROOK;
            case BLACK_QUEEN  -> PieceType.B_QUEEN;
            case BLACK_KING   -> PieceType.B_KING;
            default           -> PieceType.NONE;
        };
    }

    private void updateGameStateFromBoard(Game game, Board board) {
        Timestamp now = new Timestamp(System.currentTimeMillis());

        if (board.isMated()) {
            Side sideToMove = board.getSideToMove();
            if (sideToMove == Side.WHITE) {
                game.setState(GameState.BLACK_WIN);
            } else {
                game.setState(GameState.WHITE_WIN);
            }
            game.setGameend(now);

        } else if (board.isDraw()) {
            game.setState(GameState.DRAW);
            game.setGameend(now);

        } else {
            Side stm = board.getSideToMove();
            if (stm == Side.WHITE) {
                game.setState(GameState.WHITE_TO_MOVE);
            } else {
                game.setState(GameState.BLACK_TO_MOVE);
            }
        }
    }
}
