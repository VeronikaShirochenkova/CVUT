package cz.cvut.dbs.services;

import cz.cvut.dbs.dao.DeveloperDAO;
import cz.cvut.dbs.dao.GameDAO;
import cz.cvut.dbs.model.Developer;
import cz.cvut.dbs.model.Game;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Date;
import java.util.Objects;

@Service
public class GameService extends BaseService<Game> {

    private final GameDAO gameDao;
    private final DeveloperDAO developerDao;
    private final EmployeeService employeeService;

    @Autowired
    public GameService(GameDAO gameDao, DeveloperDAO developerDao, EmployeeService employeeService) {
        this.gameDao = gameDao;
        this.developerDao = developerDao;
        this.employeeService = employeeService;
    }

    @Override
    public GameDAO getDao() {
        return gameDao;
    }

    @Transactional
    public void create(int id, String jenre, String name, Date releaseDate) {
        if (find(id) != null) {
            System.err.println("Game with this Id:" + id +" already exist");
            return;
        }
        Game game = new Game();
        game.setId(id);
        game.setName(name);
        game.setJenre(jenre);
        game.setReleaseDate(releaseDate);
        this.persist(game);
        System.out.println("Game with this Id:" + id +" has been created");
    }

    @Transactional
    public int numberOfDevelopers(int gameId){
        Game game = find(gameId);
        return game.getDevelopers().size();
    }

    @Transactional
    public boolean hasDeveloper(int gameId, int developerId){
        Game game = find(gameId);
        Developer developer = (Developer) employeeService.find(developerId);
        return game.getDevelopers().contains(developer);
    }

//    @Transactional
//    public void addDeveloper(int gameId, int developerId) {
//        Game game = find(gameId);
//        Developer developer = (Developer) employeeService.find(developerId);
//        if (game.getDevelopers().contains(developer)){
//            System.err.println("This game already has this developer");
//            return;
//        }
//        game.addDeveloper(developer);
//        update(game);
//        employeeService.update(developer);
//        System.out.println("developer was added to game " + game.getName());
//    }
//
//    @Transactional
//    public void removeDeveloper(int gameId, int developerId) {
//        Game game = find(gameId);
//        Developer developer = (Developer) employeeService.find(developerId);
//        if (game.getDevelopers().isEmpty()) {
//            System.err.println("This game doesn't have developers");
//            return;
//        } else if (!game.getDevelopers().contains(developer)) {
//            System.err.println("This game doesn't have this developer");
//            return;
//        }
//        game.removeDeveloper(developer);
//        update(game);
//    }
}
