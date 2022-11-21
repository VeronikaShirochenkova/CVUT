package cz.cvut.dbs.model;

import lombok.Getter;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Getter
@Setter
@Table(name = "employee", schema = "public", catalog = "shirover")
public abstract class Employee {

    @Id
    @Column(name = "id", nullable = false)
    private int id;
}
