package cz.cvut.dbs.model;

import lombok.Getter;
import lombok.Setter;

import javax.persistence.*;
import java.sql.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@Entity
@Getter
@Setter
@Table(name = "game", schema = "public", catalog = "shirover")
public class Game {

//    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    private int id;

    @Column(name = "name", nullable = false, length = 50)
    private String name;

    @Column(name = "release_date", nullable = false)
    private Date releaseDate;

    @Basic
    @Column(name = "jenre", nullable = false, length = 50)
    private String jenre;

    @ManyToMany(mappedBy = "games")
    private List<Developer> developers;

    public void addDeveloper(Developer developer) {
        Objects.requireNonNull(developer);
        if (developers == null) {
            this.developers = new ArrayList<>();
        }
        developers.add(developer);
    }

    public void removeDeveloper(Developer developer) {
        Objects.requireNonNull(developer);
        developers.removeIf(c -> Objects.equals(c.getId(), developer.getId()));
    }
}
