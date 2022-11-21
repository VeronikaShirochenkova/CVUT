package cz.cvut.dbs.services;

import cz.cvut.dbs.dao.BaseDAO;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;

public abstract class BaseService<T> {

    protected abstract BaseDAO<T> getDao();

    @Transactional
    public List<T> findAll() {
        return getDao().findAll();
    }

    @Transactional
    public T find(Integer id){ return getDao().find(id); }

    @Transactional
    public void persist(T instance) {
        Objects.requireNonNull(instance);
        getDao().persist(instance);
    }

    @Transactional
    public void update(T instance) {
        Objects.requireNonNull(instance);
        getDao().update(instance);
    }

    @Transactional
    public void delete(T instance) {
        Objects.requireNonNull(instance);
        getDao().remove(instance);
    }
}
