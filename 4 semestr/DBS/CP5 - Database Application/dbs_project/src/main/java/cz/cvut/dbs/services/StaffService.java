package cz.cvut.dbs.services;

import cz.cvut.dbs.dao.StaffDAO;
import cz.cvut.dbs.model.Staff;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class StaffService extends BaseService<Staff>{

    private final StaffDAO dao;

    @Autowired
    public StaffService(StaffDAO dao) {
        this.dao = dao;
    }

    @Override
    public StaffDAO getDao() {
        return dao;
    }
}
