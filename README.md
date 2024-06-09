# Fantasy Quest

Fantasy Quest is a text-based adventure game where you, as the player, embark on a perilous journey to defeat a menacing dragon terrorizing your village. Along the way, you will encounter various challenges, make critical decisions, and engage in battles with fierce enemies.

The game utilizes the concepts of Object-Oriented Programming (OOP) to model different entities and scenarios in the game world. Here's a brief overview of how OOP is utilized:

- **Classes**: The game defines classes such as `Entity`, `Player`, `Enemy`, `Scenario`, `ChoiceScenario`, and `BattleScenario` to represent different entities and scenarios in the game.
- **Inheritance**: Inheritance is used to establish an "is-a" relationship between classes. For example, `Player` and `Enemy` classes inherit from the `Entity` class, allowing them to inherit common attributes and methods.
- **Polymorphism**: Polymorphism is achieved through virtual functions. For instance, both `Player` and `Enemy` classes have a virtual function `attack`, which is overridden to implement specific behavior for each entity type.

## Screenshots
![1](https://github.com/alifaizanchughtai/Fantasy-Quest-Game/assets/139624731/1b85c385-63ca-43af-a599-edb7f012f6b9)
![2](https://github.com/alifaizanchughtai/Fantasy-Quest-Game/assets/139624731/e704b791-d011-43a2-a973-b4519f368141)

## Gameplay

To play the game, simply compile and run the provided code. You will be prompted to enter your name and then embark on your quest to defeat the dragon. Follow the on-screen prompts, make choices, engage in battles, and ultimately decide the fate of your village!

---

### Note

This game is text-based and does not have graphical elements. Screenshots are not available in the traditional sense, but you can capture command-line outputs during gameplay to showcase the experience.
