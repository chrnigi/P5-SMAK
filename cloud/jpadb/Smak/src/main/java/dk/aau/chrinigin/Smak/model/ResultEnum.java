package dk.aau.chrinigin.Smak.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonValue;

public enum ResultEnum {
    xwin("xwin"), 
    owin("owin"), 
    draw("draw"), 
    inprogress("inprogress"), 
    notstarted("notstarted"), 
    error("error");

    private String jsonvalue;

    ResultEnum(@JsonProperty String jsonvalue) {
        this.jsonvalue = jsonvalue;
    }

    @JsonValue
    public String getJsonValue() {
        return jsonvalue;
    }
}
