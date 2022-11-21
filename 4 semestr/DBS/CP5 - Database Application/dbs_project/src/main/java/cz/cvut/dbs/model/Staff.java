package cz.cvut.dbs.model;

import lombok.Getter;
import lombok.Setter;

import javax.persistence.*;

@Entity
@Getter
@Setter
@Table(name = "staff")
public class Staff extends Employee {

    @Basic
    @Column(name = "speciality")
    private String speciality;
}
