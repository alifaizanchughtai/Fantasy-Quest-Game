#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

void slowPrint(const string &message, int millis_per_char = 50)
{
    for (char c : message)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(millis_per_char));
    }
    cout << endl;
}

class Entity
{
protected:
    string name;
    int health;
    int attackPower;

public:
    Entity(string name, int health, int attackPower)
        : name(name), health(health), attackPower(attackPower) {}

    virtual void attack(Entity &target)
    {
        int damageDealt = min(attackPower, target.getHealth());
        target.receiveDamage(damageDealt);
        slowPrint("\033[37m" + name + " attacks " + target.getName() + " for " + to_string(damageDealt) + " damage!\033[0m");
        if (!target.isAlive())
        {
            slowPrint("\033[31m" + target.getName() + " is defeated!\033[0m");
        }
    }

    virtual void receiveDamage(int damage)
    {
        health -= damage;
    }

    bool isAlive() const
    {
        return health > 0;
    }

    string getName() const
    {
        return name;
    }

    int getAttackPower() const
    {
        return attackPower;
    }

    virtual int getHealth() const
    {
        return health;
    }
};

class Player : public Entity
{
public:
    Player(string name, int health, int attackPower)
        : Entity(name, health, attackPower) {}

    void heal(int amount)
    {
        health += amount;
        slowPrint("\033[37m" + name + " heals for " + to_string(amount) + " health!\033[0m");
    }

    int getHealth() const override
    {
        return Entity::getHealth();
    }
};

class Enemy : public Entity
{
public:
    Enemy(string name, int health, int attackPower)
        : Entity(name, health, attackPower) {}

    int getHealth() const override
    {
        return Entity::getHealth();
    }
};

class Scenario
{
public:
    virtual void playScenario(Player &player) = 0;
    virtual ~Scenario() {}
};

class ChoiceScenario : public Scenario
{
private:
    string description;
    vector<string> choices;
    vector<int> outcomes;
    vector<string> results;

public:
    ChoiceScenario(string description, vector<string> choices, vector<int> outcomes, vector<string> results)
        : description(description), choices(choices), outcomes(outcomes), results(results) {}

    void playScenario(Player &player) override
    {
        slowPrint("\033[37m" + description + "\033[0m");
        for (size_t i = 0; i < choices.size(); ++i)
        {
            slowPrint("\033[36m" + string("[") + to_string(i + 1) + string("] ") + choices[i] + "\033[0m");
        }
        int choice;
        cin >> choice;
        choice--;
        if (choice >= 0 && choice < outcomes.size())
        {
            if (outcomes[choice] != 0)
            {
                player.receiveDamage(-outcomes[choice]);
            }
            slowPrint("\033[37m" + results[choice] + "\033[0m");
        }
        else
        {
            slowPrint("\033[31mInvalid choice.\033[0m");
        }
    }
};

class BattleScenario : public Scenario
{
private:
    Enemy enemy;

public:
    BattleScenario(Enemy enemy) : enemy(enemy) {}

    void playScenario(Player &player) override
    {
        slowPrint("\033[37mYou encounter the " + enemy.getName() + "!\033[0m");
        while (player.isAlive() && enemy.isAlive())
        {
            slowPrint("\033[37mChoose your action:\033[0m");
            slowPrint("\033[36m[1] Attack\033[0m");
            slowPrint("\033[36m[2] Heal\033[0m");
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
                player.attack(enemy);
                break;
            case 2:
                player.heal(10);
                break;
            default:
                slowPrint("\033[31mInvalid choice!\033[0m");
                break;
            }
            if (enemy.isAlive())
            {
                enemy.attack(player);
            }
            slowPrint("\033[37m" + player.getName() + " Health: " + to_string(player.getHealth()) + "\033[0m");
            slowPrint("\033[37m" + enemy.getName() + " Health: " + to_string(enemy.getHealth()) + "\033[0m");
        }
        if (player.isAlive())
        {
            slowPrint("\033[32mYou have defeated the " + enemy.getName() + "!\033[0m");
        }
        else
        {
            slowPrint("\033[31mYou have been defeated by the " + enemy.getName() + ".\033[0m");
        }
    }
};

class Game
{
private:
    Player player;
    vector<Scenario *> scenarios;

public:
    Game(string playerName)
        : player(Player(playerName, 100, 20))
    {
        initializeScenarios();
    }

    ~Game()
    {
        for (Scenario *scenario : scenarios)
        {
            delete scenario;
        }
    }

    void initializeScenarios()
    {
        scenarios.push_back(new ChoiceScenario(
            "Soon into the journey, you find a fork in the road.\nThe left path looks calm and safe but is a longer path to the Murky Mountain.\nThe right path looks dangerous but takes you straight to the Murky Mountain.",
            {"Take the left path", "Take the right path"},
            {-10, 0},
            {"You chose the left path and added more distance to the journey. This will cost you in terms of health eventually.",
             "The right path looks a bit dangerous, but you still decide to take it as it will help you reach there quickly."}));

        scenarios.push_back(new BattleScenario(Enemy("Goblin", 30, 10)));

        scenarios.push_back(new ChoiceScenario(
            "You continue on your journey after killing the goblin and come across a suspicious looking chest.",
            {"Open the chest", "Ignore the chest"},
            {20, 0},
            {"You find a potion inside and your health increases.",
             "You missed out on a useful item and regret it."}));

        scenarios.push_back(new ChoiceScenario(
            "After passing the chest, you encounter a wise old man who offers you advice.",
            {"Listen to the advice", "Ignore the old man and save your time"},
            {0, 0},
            {"The old man advises you to avoid the cave ahead. This might be useful later.",
             "You ignore the old man and continue on your journey."}));

        scenarios.push_back(new ChoiceScenario(
            "Soon afterwards you come across a rickety bridge over a ravine. You can either cross it while taking the risk or look for some other way around which will cost you time.",
            {"Cross the bridge", "Find another way around"},
            {-15, 0},
            {"The bridge collapses and you fall down in a ravine. You end up climbing back but lose some of your health.",
             "You decide to find another way around, avoiding potential danger."}));

        scenarios.push_back(new ChoiceScenario(
            "You eventually get through the ravine and discover a cave which seems suitable to spend the night and rest to gain back lost health.",
            {"Enter the cave", "Continue on your path"},
            {-10, 0},
            {"You were attacked by bats and lose health.",
             "You continue safely on your journey."}));

        scenarios.push_back(new ChoiceScenario(
            "Moving on you see a stranger in need of help. You have already short on time and it is getting dark.",
            {"Help the stranger", "Ignore the stranger"},
            {20, 0},
            {"The stranger rewards you for your kindness with some food. You eat the food and your health increases.",
             "You ignore the stranger and they curse you."}));

        scenarios.push_back(new ChoiceScenario(
            "It's nearly nightfall and you come across and place suitable for a fire.",
            {"Rest by the fire and spend the night", "Don't rest and move on quickly"},
            {15, 0},
            {"You wake up the next day feeling rejuvenated. Your health increases.",
             "You move on quickly, avoiding the rest."}));

        scenarios.push_back(new ChoiceScenario(
            "Continuing on the journey, you find a mysterious amulet on the ground.",
            {"Pick it up", "Leave it"},
            {0, -15},
            {"The amulet does nothing special.",
             "A curse falls upon you for leaving the amulet. You lose health."}));

        scenarios.push_back(new BattleScenario(Enemy("Troll", 50, 15)));

        scenarios.push_back(new ChoiceScenario(
            "After the intense encounter with the troll, you carry on the journey and come across a shortcut that goes through a dark forest.",
            {"Don't take the shortcut", "Take the shortcut"},
            {0, 0},
            {"You decide to take the long route.",
             "You take the shortcut and continue on the journey."}));

        scenarios.push_back(new ChoiceScenario(
            "You encounter a river with a strong current. You see some planks near the river bank as well.",
            {"Swim across the river", "Build a raft and then cross the river using it"},
            {-15, 20},
            {"The current is too strong. You lose health.",
             "You successfully build a raft and gain health from the rest."}));

        scenarios.push_back(new ChoiceScenario(
            "You find some wild berries along your way. You haven't seen any berries like these before, hence, you are unsure whether to take them or leave them.",
            {"Eat the berries", "Ignore the berries"},
            {10, 0},
            {"You eat the berries and gain health.",
             "You ignore the berries and move on."}));

        scenarios.push_back(new ChoiceScenario(
            "You come across an ancient shrine.",
            {"Pray at the shrine for the betterment of your journey", "Ignore the shrine"},
            {0, 0},
            {"You feel a strange energy but nothing happens.",
             "You ignore the shrine and move on."}));

        scenarios.push_back(new ChoiceScenario(
            "Finally you reach the Murky Mountain. Considering how difficult the journey till now was, you are having second thoughts as to whether you should really face the dragon or not.",
            {"Turn back now", "Man up and face the dragon"},
            {-1000, 0},
            {"You chose to turn back. What a coward you are.",
             "You: Let's show the dragon who's the boss!\nDragon: Who dares awaken me from my slumber?\nYou: The one destined to end your existence.\nDragon: HaHaHa! That's what those who came before you claimed.\nYou: No more words. It's time for you to meet your end!"}));

        scenarios.push_back(new BattleScenario(Enemy("Dragon", 100, 25)));
    }

    void start()
    {
        displayBanner();
        getPlayerName();
        displayIntroduction();

        for (Scenario *scenario : scenarios)
        {
            if (!player.isAlive())
            {
                break;
            }
            scenario->playScenario(player);
        }

        if (player.isAlive())
        {
            slowPrint("\033[32mCongratulations! You have completed your quest and defeated the dragon!\033[0m");
            slowPrint("\033[37mYou return back home and break the news to the villagers.\033[0m");
            slowPrint("\033[37mListening to this everyone is overwhelmed with joy.\033[0m");
            slowPrint("\033[37mYou look back at the Murky Mountain, with pride in your eyes that you have finished it once and for all.\033[0m");
            slowPrint("\033[37mThe village and its inhabitants set to live happily ever after.\033[0m");
            slowPrint("\033[37m**********THE END**********\033[0m");
        }
        else
        {
            slowPrint("\033[31mGame Over! Your journey ends here.\033[0m");
        }
    }

    void displayBanner()
    {
        cout << "\033[37m*********************************************************************************\033[0m" << endl;
        cout << "\033[37m*********************************************************************************\033[0m" << endl;
        cout << "\033[37m*****************************     FANTASY QUEST     *****************************\033[0m" << endl;
        cout << "\033[37m*********************************************************************************\033[0m" << endl;
        cout << "\033[37m*********************************************************************************\033[0m" << endl;
    }

    void getPlayerName()
    {
        slowPrint("\033[37mO Brave Adventurer! Say your name to enter the world of Fantasy Quest: \033[0m");
        string name;
        cin >> name;
        player = Player(name, 100, 20);
        slowPrint("\033[37mWelcome, " + name + "!\033[0m");
    }

    void displayIntroduction()
    {
        slowPrint("\033[37mYou belong to a small village where a dragon wreaks havoc every once in a while.\033[0m");
        slowPrint("\033[37mYou have decided to set out on a journey to kill the dragon once and for all.\033[0m");
        slowPrint("\033[37mBut the journey is not easy, it is said that the ones who went out in the past to kill the dragon have never returned.\033[0m");
        slowPrint("\033[37mThere are mountains to climb and rivers to cross, deep forests to surpass and many vile creatures to fight.\033[0m");
        slowPrint("\033[37mHowever, you as the brave adventurer embark on this journey to become the hero of your village.\033[0m");
        slowPrint("\033[37mGathering all the supplies and weapons you might be needing, you set towards the Murky Mountain, home to the beast you are set to kill.\033[0m");
    }
};

int main()
{
    Game game("Player");
    game.start();
    return 0;
}
