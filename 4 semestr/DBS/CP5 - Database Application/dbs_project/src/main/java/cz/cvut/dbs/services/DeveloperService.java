package cz.cvut.dbs.services;

import cz.cvut.dbs.dao.DeveloperDAO;
import cz.cvut.dbs.model.Developer;
import cz.cvut.dbs.model.Game;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

@Service
public class DeveloperService extends BaseService<Developer>{

    private final DeveloperDAO dao;
    private final EmployeeService employeeService;
    private final GameService gameService;

    @Autowired
    public DeveloperService(DeveloperDAO dao, EmployeeService employeeService, GameService gameService) {
        this.dao = dao;
        this.employeeService = employeeService;
        this.gameService = gameService;
    }

    @Override
    public DeveloperDAO getDao() {
        return dao;
    }

    @Transactional
    public void create(int id, String level){
        if(dao.exists(id)){
            System.err.println("Developer with this Id:" + id +" already exist");
            return;
        }
       Developer developer = new Developer();
        developer.setId(id);
        developer.setLevel(level);
        this.persist(developer);
        System.out.println("Developer with this Id:" + id +" has been created");
    }

    @Transactional
    public List<Developer> findAllSeniors(){
        System.out.println("Search for all developers with senior level");
        return dao.findAll().stream().filter(developer -> (Objects.equals(developer.getLevel(), "senior"))).collect(Collectors.toList());
    }

    @Transactional
    public void addGame(int developerId, int gameId) {
        Developer developer = (Developer) employeeService.find(developerId);
        Game game = gameService.find(gameId);
        if (developer.getGames().contains(game)){
            System.err.println("This developer already has this game");
            return;
        }
        developer.addGame(game);
        update(developer);
        System.out.println("game was added to developer " + game.getId());
    }

    @Transactional
    public void removeGame(int developerId, int gameId) {
        Developer developer = (Developer) employeeService.find(developerId);
        Game game = gameService.find(gameId);
        if (developer.getGames().isEmpty()) {
            System.err.println("This developer doesn't have games");
            return;
        } else if (!game.getDevelopers().contains(developer)) {
            System.err.println("This game doesn't have this game");
            return;
        }
        developer.removeGame(game);
        update(developer);
        System.out.println("Game \"" + game.getName() +  "\" was deleted from developer " + game.getId());
    }
}
