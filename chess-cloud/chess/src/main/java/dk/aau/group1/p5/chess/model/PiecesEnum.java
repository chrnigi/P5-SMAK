package dk.aau.group1.p5.chess.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonValue;

public enum PiecesEnum {
    KING("K"),
    QUEEN("Q"),
    ROOK("R"),
    BISHOP("B"),
    KNIGHT("N"),
    PAWN("P"),
    NONE("Z");

    
    private String jsonvalue;
    
    PiecesEnum(@JsonProperty String jsonvalue) {
        this.jsonvalue = jsonvalue;
    }
    
    @JsonValue
    public String getJsonvalue() {
        return jsonvalue;
    }
}
