package cz.cvut.dbs.dao;

import cz.cvut.dbs.model.Developer;
import org.springframework.stereotype.Repository;

@Repository
public class DeveloperDAO extends BaseDAO<Developer>{

    public DeveloperDAO() {
        super(Developer.class);
    }
}
