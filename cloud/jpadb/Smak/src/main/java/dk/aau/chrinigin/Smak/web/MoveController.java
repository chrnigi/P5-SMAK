package dk.aau.chrinigin.Smak.web;

import dk.aau.chrinigin.Smak.model.Game;
import dk.aau.chrinigin.Smak.model.ResultEnum;
import dk.aau.chrinigin.Smak.model.Move;
import dk.aau.chrinigin.Smak.repository.GameRepository;
import dk.aau.chrinigin.Smak.service.MoveService;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.sql.Timestamp;
import com.github.bhlangonijr.chesslib.Board;
import com.github.bhlangonijr.chesslib.Square;
import com.github.bhlangonijr.chesslib.Side;
import com.github.bhlangonijr.chesslib.Piece;

import java.util.List;
// Handles all move operations: applying moves, generating new FEN via chesslib,
// updating PGN, and detecting check/checkmate or draws. 
// Example: POST /moves → chesslib applies UCI "e2e4", new FEN is saved.
@RestController
public class MoveController {

    private final MoveService moveService;
    private final GameRepository gameRepository;

    public MoveController(MoveService moveService, GameRepository gameRepository) {
        this.moveService = moveService;
        this.gameRepository = gameRepository;
    }

    @GetMapping("/moves/ping")
    public String movesPing() { return "MoveController is alive"; }

    @GetMapping("/moves")
    List<Move> all() { return moveService.findAll(); }

    @GetMapping("/moves/{id}")
    List<Move> byGame(@PathVariable Long id) { return moveService.getByGameId(id); }

    @PostMapping("/moves")
    Move newMove(@RequestBody Move move) {
        if (move.getFenAfter() != null && !move.getFenAfter().isEmpty()) {
            Game game = gameRepository.findById(move.getId())
                    .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + move.getId()));

            Board b = new Board();
            b.loadFromFen(move.getFenAfter());
            inferMoveFlagsFromSan(move);
            updateResultFromBoardOrSan(b, game, move);

            game.setLatestFen(move.getFenAfter());
            game.setPgn(appendSanToPgn(game.getPgn(), move.getPlyno(), move.getSan() == null ? move.getUci() : move.getSan()));
            gameRepository.save(game);
            return moveService.save(move);
        }

        Game game = gameRepository.findById(move.getId())
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Game not found: " + move.getId()));
        List<Move> existing = moveService.getByGameId(move.getId());
        String currentFen = existing.isEmpty() ? game.getInitialFen() : existing.get(existing.size() - 1).getFenAfter();

        Board board = new Board();
        board.loadFromFen(currentFen);

        String uci = move.getUci();
        if (uci == null || (uci.length() != 4 && uci.length() != 5))
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "UCI must be 4 or 5 chars, e.g., e2e4 or a7a8q");

        Square from = Square.fromValue(uci.substring(0, 2).toUpperCase());
        Square to = Square.fromValue(uci.substring(2, 4).toUpperCase());

        com.github.bhlangonijr.chesslib.move.Move chessMove;
        if (uci.length() == 5) {
            char prom = Character.toLowerCase(uci.charAt(4));
            Side side = board.getSideToMove();
            Piece promoPiece = switch (prom) {
                case 'q' -> (side == Side.WHITE ? Piece.WHITE_QUEEN : Piece.BLACK_QUEEN);
                case 'r' -> (side == Side.WHITE ? Piece.WHITE_ROOK : Piece.BLACK_ROOK);
                case 'b' -> (side == Side.WHITE ? Piece.WHITE_BISHOP : Piece.BLACK_BISHOP);
                case 'n' -> (side == Side.WHITE ? Piece.WHITE_KNIGHT : Piece.BLACK_KNIGHT);
                default -> throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Invalid promotion piece (use q/r/b/n): " + prom);
            };
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to, promoPiece);
        } else {
            chessMove = new com.github.bhlangonijr.chesslib.move.Move(from, to);
        }

        boolean ok = board.doMove(chessMove);
        if (!ok) throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Illegal move: " + uci);

        move.setFenAfter(board.getFen());
        inferMoveFlagsFromSan(move);
        updateResultFromBoardOrSan(board, game, move);

        game.setLatestFen(move.getFenAfter());
        game.setPgn(appendSanToPgn(game.getPgn(), move.getPlyno(), move.getSan() == null ? uci : move.getSan()));
        gameRepository.save(game);
        return moveService.save(move);
    }

    private void updateResultFromBoardOrSan(Board board, Game game, Move move) {
        Timestamp now = new Timestamp(System.currentTimeMillis());

        boolean sanMate = move.getSan() != null && move.getSan().contains("#");
        boolean sanCheck = move.getSan() != null && move.getSan().contains("+");

        if (board.isMated() || sanMate || Boolean.TRUE.equals(move.getIsMate())) {
            game.setResult(board.getSideToMove() == Side.WHITE ? ResultEnum.BLACKWIN : ResultEnum.WHITEWIN);
            game.setGameend(now);
            move.setIsMate(true);
        } else if (board.isDraw()) {
            game.setResult(ResultEnum.DRAW);
            game.setGameend(now);
        } else {
            game.setResult(ResultEnum.INPROGRESS);
        }

        if (sanCheck && !Boolean.TRUE.equals(move.getIsMate())) {
            move.setIsCheck(true);
        }
    }

    private void inferMoveFlagsFromSan(Move move) {
        if (move.getSan() == null) return;
        if (move.getSan().contains("#")) move.setIsMate(true);
        else if (move.getSan().contains("+")) move.setIsCheck(true);
    }

    private static int toMoveNumber(int plyno) { return (plyno + 1) / 2; }

    private static String appendSanToPgn(String current, int plyno, String sanOrUci) {
        String san = (sanOrUci == null) ? "" : sanOrUci.trim();
        StringBuilder sb = new StringBuilder(current == null ? "" : current.trim());
        boolean whiteToMove = (plyno % 2 == 1);
        if (sb.length() > 0) sb.append(' ');
        if (whiteToMove) sb.append(toMoveNumber(plyno)).append(". ").append(san);
        else sb.append(san);
        return sb.toString();
    }
}
