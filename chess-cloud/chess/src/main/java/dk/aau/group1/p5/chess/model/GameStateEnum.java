package dk.aau.group1.p5.chess.model;

import com.fasterxml.jackson.annotation.JsonProperty;

public enum GameStateEnum {
    WHITE_WIN("WHITE_WIN"),
    BLACK_WIN("BLACK_WIN"),
    DRAW_AGREED("DRAW_AGREED"),
    DRAW_REPETITION("DRAW_REPETITION"),
    DRAW_FIFTY_MOVES("DRAW_FIFTY_MOVES"),
    WHITE_TO_MOVE("WHITE_TO_MOVE"),
    BLACK_TO_MOVE("BLACK_TO_MOVE"),
    NOT_STARTED("NOT_STARTED"),
    ERROR("ERROR");

    private String jsonvalue;

    GameStateEnum(@JsonProperty String jsonvalue) {
        this.jsonvalue = jsonvalue;
    }

    public String getJsonValue() {
        return jsonvalue;
    }
}
