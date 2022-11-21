package cz.cvut.dbs.dao;

import cz.cvut.dbs.model.Staff;
import org.springframework.stereotype.Repository;

@Repository
public class StaffDAO extends BaseDAO<Staff> {

    public StaffDAO() {
        super(Staff.class);
    }
}
