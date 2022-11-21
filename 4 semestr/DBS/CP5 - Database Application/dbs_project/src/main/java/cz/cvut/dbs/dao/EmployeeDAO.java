package cz.cvut.dbs.dao;

import cz.cvut.dbs.model.Employee;
import org.springframework.stereotype.Repository;

@Repository
public class EmployeeDAO extends BaseDAO<Employee> {

    public EmployeeDAO() {
        super(Employee.class);
    }
}
