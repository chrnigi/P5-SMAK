package dk.aau.group1.p5.chess.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonValue;

public enum MoveTypeEnum {
    PROMOTION("promotion"),
    ENPASSANT("enpassant"),
    CASTLES("castling"),
    NORMAL("normal"),
    NULLMOVE("nullmove");


    private String jsonvalue;

    MoveTypeEnum(@JsonProperty String jsonvalue) {
        this.jsonvalue = jsonvalue;
    }

    @JsonValue
    public String getJsonValue() {
        return jsonvalue;
    }

}
