package cz.cvut.dbs;

import cz.cvut.dbs.model.Developer;
import cz.cvut.dbs.model.Game;
import cz.cvut.dbs.services.DeveloperService;
import cz.cvut.dbs.services.EmployeeService;
import cz.cvut.dbs.services.GameService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import java.sql.Date;
import java.util.List;

@Component
public class Init {

    private final DeveloperService developerService;
    private final GameService gameService;
    private final EmployeeService employeeService;

    @Autowired
    public Init(DeveloperService developerService, GameService gameService, EmployeeService employeeService) {
        this.developerService = developerService;
        this.gameService = gameService;
        this.employeeService = employeeService;
    }

    @PostConstruct
    public void initApp() {
        //create new developer
        developerService.create(44846660, "senior");
        //this developer already exists
        developerService.create(44846660, "senior");
        //find this developer and print his level
        Developer developer = developerService.find(44846660);
        System.out.println(developer.getLevel());

        //create new game
        gameService.create(4, "platformer", "Mario", Date.valueOf("2021-11-14"));
        //this game already exists
        gameService.create(4, "platformer", "Mario", Date.valueOf("2021-11-14"));
        //find this game and print its name
        Game game = gameService.find(4);
        System.out.println(game.getName());

        //adding game to developer
        developerService.addGame(developer.getId(), game.getId());
        //this developer already has this game
        developerService.addGame(developer.getId(), game.getId());
        System.out.println(game.getName()+ " has " + gameService.numberOfDevelopers(game.getId()) + " developers");


        //All developers and senior developers
        List<Developer> developers = developerService.findAll();
        List<Developer> seniors = developerService.findAllSeniors();
        System.out.println("Number of senior developers: " + seniors.size());
        System.out.println("Number of all developers: " + developers.size());

        //=============Clearing changes=================
        developerService.removeGame(developer.getId(), game.getId());
        System.out.println(game.getName()+ " has " + gameService.numberOfDevelopers(game.getId()) + " developers");
        employeeService.delete(developer);
        gameService.delete(game);
    }
}
