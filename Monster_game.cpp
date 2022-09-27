#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature
{   protected:

    std::string m_name;
    char m_symbol;
    int m_health;
    int m_damage;
    int m_gold;
    public:

    Creature(std::string name, char symbol, int health, int damage, int gold):
        m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
    {
    }

    std::string getName() const {return m_name;}
    char getSymbol() const {return m_symbol;}
    int getHealth() const {return m_health;}
    int getDamage() const {return m_damage;}
    int getGold() const {return m_gold;}

    void reduceHealth(int damage) {m_health -= damage;}
    bool isDead() {return m_health <= 0;}
    void addGold(int gold) {m_gold += gold;}
};

class Player: public Creature
{
    int m_level;
    public:

    Player(const std::string &name): Creature(name, '@', 10, 1, 0), m_level{1}
    {
    }

    void levelUp()
    {
        ++m_level;
        ++m_damage;
    }
    int getLevel() const {return m_level;}
    bool hasWon() const {return m_level >= 20;}
};

class Monster: public Creature
{
    public:

    enum Type{
        DRAGON,
        ORC,
        SLIME,
        MAX_TYPES
    };

    struct MonsterData
    {
        std::string name;
        char symbol;
        int health;
        int damage;
        int gold;
    };

    static MonsterData monsterData[MAX_TYPES];

    Monster(Type type): Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
    {
    }

    static Type getRandomMonster()
    {
        return static_cast<Type>(getRandomNumber(0, MAX_TYPES - 1));
    }
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
    {"dragon", 'D', 20, 4, 100},
    {"orc", 'O', 4, 2, 25},
    {"slime", 'S', 1, 1, 10}
};

void fightMonster(Player &player, Monster &monster);
void attackMonster(Player &player, Monster &monster);
void attackPlayer(Player &player, Monster &monster);
char checkingTheInput();

int main()
{
    using namespace std;
    srand(static_cast<unsigned int>(time(0)));
    rand();

    cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    Player player(name);

    cout << "Welcome, " << player.getName() << "!\n";
    cout << "You have " << player.getHealth() << " health and are carrying " << player.getGold() << " gold\n";

    while(!player.hasWon() && !player.isDead())
    {
        Monster monster = Monster::getRandomMonster();
        cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ")\n";
        fightMonster(player, monster);
    }

    if(player.hasWon())
        cout << "Congratulations, you have reached level 20, you have won the game.\n";

    else if(player.isDead())
        cout << "You're dead.\n"
             << "You had level " << player.getLevel() << " and " << player.getGold() << " gold.\n";

    cout << "Thank you for playing my game.\n";

    return 0;
}

void fightMonster(Player &player, Monster &monster)
{
    using namespace std;

    while(!player.isDead() && !monster.isDead())
    {
        cout << "(R)un or (F)ight: ";
        char ch = checkingTheInput();

        if(ch == 'r')
        {
            int i = getRandomNumber(1, 100);
            if(i <= 50 && i > 0)
            {
                cout << "You successfully fled.\n";
                break;
            }
            else
            {
                cout << "Escape failed!\n";
                attackPlayer(player, monster);
            }
        }

        else if(ch == 'f')
        {
            attackMonster(player, monster);
        }
    }

}

char checkingTheInput()
{
    char ch;

    while(tolower(ch) != 'r' && tolower(ch) != 'f')
    {
        std::cin >> ch;

        if(std::cin.fail())
            std::cin.clear();

        std::cin.ignore(32767, '\n');
    }

    return ch;
}

void attackPlayer(Player &player, Monster &monster)
{
    player.reduceHealth(monster.getDamage());
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";

}

void attackMonster(Player &player, Monster &monster)
{
    monster.reduceHealth(player.getDamage());
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";

    if(monster.isDead())
    {
        player.levelUp();
        player.addGold(monster.getGold());
        std::cout << " You killed the " << monster.getName() << ".\n";
        std::cout << "You have raised yor level!\n"
                  << "Your level is " << player.getLevel() << ".\n";
        std::cout << "You found " << monster.getGold() << ".\n" << "You have " << player.getGold() << " gold.\n";
    }
    else
    {
        std::cout << "The enemy is not defeated!\n";
        attackPlayer(player, monster);
    }
}
