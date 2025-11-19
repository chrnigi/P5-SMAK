package dk.aau.chrinigin.Smak;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class SmakApplication {

	public static void main(String[] args) {
		SpringApplication.run(SmakApplication.class, args);
	}

}

/* 
This is how to create a new game and post moves to it using PowerShell
with the NEW backend.

# 1) Create a new game
# We just post an empty JSON body, the backend sets start time and state automatically.

$g  = irm -Method Post http://localhost:8080/games -ContentType "application/json" -Body '{}'
$id = $g.id

# 2) Post moves
# We now ONLY send UCI. The backend figures out piece types and promotions itself.

function Post-Move([int]$ply,[string]$uci) {
    $body = @{
        id    = $id      # game id
        plyno = $ply     # half-move number
        uci   = $uci     # move in UCI notation, e.g. "e2e4"
        # pieceMoved    = "PAWN"   # optional: PieceType enum
        # pieceCaptured = "NONE"   # optional: PieceType enum (or NONE)
        # promotion     = $true    # optional: only if this move promotes (always to queen)
    } | ConvertTo-Json

    irm -Method Post http://localhost:8080/moves -ContentType "application/json" -Body $body
}

# 3) Example game (Scholar's Mate) using ONLY UCI
Post-Move 1 "e2e4"
Post-Move 2 "e7e5"
Post-Move 3 "g1f3"
Post-Move 4 "b8c6"
Post-Move 5 "f1c4"
Post-Move 6 "g8f6"
Post-Move 7 "d1f3"
Post-Move 8 "f8c5"
Post-Move 9 "f3f7"  # Qxf7
*/
