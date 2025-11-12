package dk.aau.group1.p5.chess.model;

import java.sql.Timestamp;
import java.time.Instant;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import jakarta.annotation.Generated;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

@Entity
@Table(name = "games")
@JsonIgnoreProperties(ignoreUnknown = true, value = {"id"}, allowGetters = true) 
public class Games {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id", nullable = false)
    private Long id;

    @Column(name = "gamestate", nullable = false)
    @Enumerated(EnumType.STRING)
    private GameStateEnum gamestate;

    @Column(name = "white_rating")
    private Short white_rating;

    @Column(name = "black_rating")
    private Short black_rating;

    @Column(name = "gamestart")
    private Timestamp gamestart;

}
