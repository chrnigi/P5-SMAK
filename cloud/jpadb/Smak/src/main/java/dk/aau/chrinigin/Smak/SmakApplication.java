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
this is how to create a new game and post moves to it using PowerShell

# new game
$g  = irm -Method Post http://localhost:8080/games -ContentType "application/json" -Body '{"result":"INPROGRESS"}'
$id = $g.id

We use the function below the command to make it easier for testing if u dont wanna use it then u gotta post each move like this 

irm -Method Post http://localhost:8080/moves -ContentType "application/json" -Body ("{""id"":$id,""plyno"":1,""uci"":""e2e4"",""san"":""e4""}")

function Post-Move([int]$ply,[string]$uci,[string]$san) {
    $body = @{ id = $id; plyno = $ply; uci = $uci; san = $san } | ConvertTo-Json
    irm -Method Post http://localhost:8080/moves -ContentType "application/json" -Body $body
	}
# This is how u play a game by posting moves one by one
Post-Move 1 "e2e4" "e4"
Post-Move 2 "e7e5" "e5"
Post-Move 3 "g1f3" "Nf3"
Post-Move 4 "b8c6" "Nc6"
Post-Move 5 "f1c4" "Bc4"
Post-Move 6 "g8f6" "Nf6"
Post-Move 7 "d1f3" "Qf3"
Post-Move 8 "f8c5" "Bc5"
Post-Move 9 "f3f7" "Qxf7#"

*/
