package cz.cvut.dbs.dao;

import cz.cvut.dbs.model.Game;
import org.springframework.stereotype.Repository;

@Repository
public class GameDAO extends BaseDAO<Game> {

    public GameDAO() {
        super(Game.class);
    }
}
