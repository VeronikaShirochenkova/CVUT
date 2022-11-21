package cz.cvut.dbs.model;

import lombok.Getter;
import lombok.Setter;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@Entity
@Getter
@Setter
@Table(name = "developer", schema = "public", catalog = "shirover")
public class Developer extends Employee {

    @Basic
    @Column(name = "level", nullable = false, length = 50)
    private String level;


    @ManyToMany
    private List<Game> games;

    public void addGame(Game game) {
        Objects.requireNonNull(game);
        if (games == null) {
            this.games = new ArrayList<>();
        }
        games.add(game);
    }

    public void removeGame(Game game) {
        Objects.requireNonNull(game);
        games.removeIf(c -> Objects.equals(c.getId(), game.getId()));
    }
}
