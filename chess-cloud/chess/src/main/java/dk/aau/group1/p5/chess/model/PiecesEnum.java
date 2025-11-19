package dk.aau.group1.p5.chess.model;

import com.fasterxml.jackson.annotation.JsonProperty;

public enum PiecesEnum {
    KING("K"),
    QUEEN("Q"),
    ROOK("R"),
    BISHOP("B"),
    KNIGHT("N"),
    PAWN("P"),
    NONE("NONE");

    
    private String jsonvalue;
    
    PiecesEnum(@JsonProperty String jsonvalue) {
        this.jsonvalue = jsonvalue;
    }
    
    public String getJsonvalue() {
        return jsonvalue;
    }
}
