package cz.cvut.dbs.services;

import cz.cvut.dbs.dao.EmployeeDAO;
import cz.cvut.dbs.model.Employee;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class EmployeeService extends BaseService<Employee>{

    private final EmployeeDAO dao;

    @Autowired
    public EmployeeService(EmployeeDAO dao) {
        this.dao = dao;
    }

    @Override
    public EmployeeDAO getDao() {
        return dao;
    }
}
