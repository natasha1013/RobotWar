// *********************************************************
// Program: main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: T11L
// Trimester: 2410
// Member_1: 1211108521 | DAMIA ALYSSA BINTI ADAM SIM | 1211108521@student.mmu.edu.my | +6012-695 7989
// Member_2: 1211108251 | MAYA BINTI MOHAMMAD AMIR HAMZAH | 1211108251@student.mmu.edu.my | +6012-984 0002
// Member_3: 1211107915 | MOHAMED IRFAN ISMAIL BIN MOHAMED AMIN | 1211107915@student.mmu.edu.my | +6011-6091 9551
// Member_4: 1211110223 | NUR ALEYA NATASHA BINTI YASIR | 1211110223@student.mmu.edu.my | +6012-640 4524
// *********************************************************
// Task Distribution
// Member_1: ReadFile, OutputFile, LinkedList,RoboTank, RobotBomber
// Member_2: Queuing Waiting Robots, Queuing Destroyed Robots, UltimateRobot, RobotLaser
// Member_3: Terminator, BlueThunder, MadBot, RobotTeleport
// Member_4: UML Diagram, Queueing Reviving Robots, RoboCop, TerminatorRoboCop
// *********************************************************

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include "Vector.h"
#include "List.h"
#include "Queue.h"
using namespace std;

class Battlefield;

class Robot
{
protected:
    int robotPositionX = -1;
    int robotPositionY = -1;
    string id_ = ""; // eg RC01, to display on the grid
    string robotType_ = "";
    string robotName_ = ""; // eg RC01_Star

    int numOfLives_ = 3;
    int numOfKills_ = 0;

    int originalIndex_;
    Battlefield* battlefield;

public:
    // PC + DC
    Robot(string id = "", int x = -1, int y = -1) : id_(id), robotPositionX(x), robotPositionY(y) {}
    virtual ~Robot(){}

    // Fetch&Manipulation ---------------------------------------------------------------------------------------------
    int x() const { return robotPositionX; }
    void setX(int x) { robotPositionX = x; }

    int y() const { return robotPositionY; }
    void setY(int y) { robotPositionY = y; }

    string id() const { return id_; }
    void setId(string id) { id_ = id; }

    string robotType() const { return robotType_; }
    void setRobotType(string robotType) { robotType_ = robotType; }

    string robotName() const { return robotName_; }
    void setRobotName(string robotName) { robotName_ = robotName; }

    int numOfLives() const { return numOfLives_; }
    void setNumOfLives(int numOfLives) { numOfLives_ = numOfLives; }

    int numOfKills() const { return numOfKills_; }
    void addNumOfKills(){
        numOfKills_++;
        cout << id_ << " total number of kills: " << numOfKills_ << "\n";
        if (numOfKills_ % 3 == 0){
            numOfKills_ = 0;
            upgrade();
        }
    }

    int originalIndex() const {return originalIndex_;}
    void setOriginalIndex(int originalIndex){originalIndex_ = originalIndex;}

    virtual void setBattlefield(Battlefield* battlefieldPtr){};

    friend ostream& operator<<(ostream &out, const Robot& r)
    {
        out << r.id_ << " at (" << r.robotPositionX << ", " << r.robotPositionY <<
        "), " << "lives: " << r.numOfLives_ << ", kills: " << r.numOfKills_ << ",";
        return out;
    }

    virtual void upgrade() = 0;
    virtual void setLocation(int x, int y) = 0;
    virtual void actions() = 0;
};

// Abilities ---------------------------------------------------------------------------------------------
class SeeingRobot: virtual public Robot
{
protected:
    int minX = -1;
    int minY = -1;
    int maxX = -1;
    int maxY = -1;

public:
    virtual ~SeeingRobot(){}
    virtual void actionLook() = 0;
};

class MovingRobot: virtual public Robot
{
protected:
    int count = 0;
    int attempts = 0, maxAttempts = 8;
    int tempX, tempY;

public:
    virtual ~MovingRobot(){}

    virtual void actionMove() = 0;
};

class ShootingRobot: virtual public Robot
{
protected:
    int distance = 0, distY, distX;
    int targetFireX, targetFireY;
    const int maxDistance = 10;

public:
    virtual ~ShootingRobot(){}

    virtual void actionFire() = 0;
};

class SteppingRobot: virtual public Robot
{
protected:
    int minRangeX, maxRangeX, minRangeY, maxRangeY;
    bool Terminated = false;
    bool robotKilled = false;
    int targetStepX, targetStepY;
    int randomX, randomY;

public:
    virtual ~SteppingRobot(){}

    virtual void actionStep() = 0;
};

class RoboCop: public SeeingRobot, public MovingRobot, public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;

public:
    RoboCop(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr) //is there enter after this
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotType_ = "RoboCop";
        isValid = true;
        numOfKills_ = 0;
        robotAutoIncrementInt_++;
    }
    virtual ~RoboCop(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY= y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void actionLook();
    virtual void actionMove();
    virtual void actionFire();
    virtual void actions();
    virtual void upgrade();
};
int RoboCop::robotAutoIncrementInt_ = 0;

class Terminator: public SeeingRobot, public MovingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;

public:
    Terminator (string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr) {
        id_ = id;
        robotPositionX = x;
        robotPositionY= y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "Terminator";
        numOfKills_ = 0;
        isValid = true;
    }
    virtual ~Terminator(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void upgrade();
    virtual void actionLook();
    virtual void actionMove();
    virtual void actions();
};
int Terminator::robotAutoIncrementInt_ = 0;

class TerminatorRoboCop: public SeeingRobot, public ShootingRobot, public SteppingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;

public:
    TerminatorRoboCop(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "TerminatorRoboCop";
        isValid = true;
        numOfKills_ = 0;
    }
    virtual ~TerminatorRoboCop(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void upgrade();
    virtual void actionLook();
    virtual void actionFire();
    virtual void actionStep();
    virtual void actions();
};
int TerminatorRoboCop::robotAutoIncrementInt_ = 0;

class BlueThunder:public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;
    int shots;

public:
    BlueThunder(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        int shots = 8;
        robotType_ = "BlueThunder";
        isValid = true;
        numOfKills_ = 0;
    }
    virtual ~BlueThunder(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void upgrade();
    virtual void actionFire();
    virtual void actions();
};
int BlueThunder::robotAutoIncrementInt_ = 0;

class Madbot: public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;

public:
    Madbot(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr){
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "Madbot";
        isValid = true;
        numOfKills_ = 0;
    }

    virtual ~Madbot(){}
    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void upgrade();
    virtual void actionFire();
    virtual void actions();
};
int Madbot::robotAutoIncrementInt_ = 0;

class RoboTank: public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;
    bool isValid;

public:
    RoboTank(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr) {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "RoboTank";
        isValid = true;
        numOfKills_ = 0;
    }
    virtual ~RoboTank(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    bool exists()const {return isValid;}

    virtual void upgrade();
    virtual void actionFire();
    virtual void actions();
};
int RoboTank::robotAutoIncrementInt_ = 0;

class UltimateRobot: public SeeingRobot, public MovingRobot, public ShootingRobot, public SteppingRobot
{
private:
    static int robotAutoIncrementInt_;
public:
    UltimateRobot(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr){
        id_ = id;
        robotPositionX = x;
        robotPositionY= y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "UltimateRobot";
        numOfKills_ = 0;
    }
    virtual ~UltimateRobot(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    virtual void upgrade(){}

    virtual void actionLook();
    virtual void actionMove();
    virtual void actionFire();
    virtual void actionStep();
    virtual void actions();
};
int UltimateRobot::robotAutoIncrementInt_ = 0;

// AdditionalRobots ---------------------------------------------------------------------------------------------
class RobotBomber: public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;

public:
    // PC & DC
    RobotBomber(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr){
        id_ = id;
        robotPositionX = x;
        robotPositionY= y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "RobotBomber";
        numOfKills_ = 0;
    }
    virtual ~RobotBomber(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    virtual void upgrade(){}

    virtual void actionFire();
    virtual void actions();
};
int RobotBomber::robotAutoIncrementInt_ = 0;

class RobotLaser:public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;
    int shots;

public:
    // PC & DC
    RobotLaser(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        shots = 8;
        robotType_ = "RobotLaser";
        numOfKills_ = 0;
    }
    virtual ~RobotLaser(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){robotPositionX = x; robotPositionY = y;}
    void setBattlefield(Battlefield* battlefieldPtr) override{battlefield = battlefieldPtr;}
    virtual void upgrade(){};

    virtual void actionFire();
    virtual void actions();
};
int RobotLaser::robotAutoIncrementInt_ = 0;

class RobotTeleport : public MovingRobot, public ShootingRobot
{
private:
    static int robotAutoIncrementInt_;

public:
    // PC & DC
    RobotTeleport(string id = "", int x = -1, int y = -1, Battlefield* battlefieldPtr = nullptr)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        battlefield = battlefieldPtr;
        robotAutoIncrementInt_++;
        robotType_ = "RobotTeleport";
        numOfKills_ = 0;
    }
    virtual ~RobotTeleport(){}

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }
    virtual void setLocation(int x, int y){ robotPositionX = x; robotPositionY = y; }
    void setBattlefield(Battlefield* battlefieldPtr) override { battlefield = battlefieldPtr; }
    virtual void upgrade(){}

    virtual void actionMove();
    virtual void actionFire();
    virtual void actions();
};
int RobotTeleport::robotAutoIncrementInt_ = 0;

class Battlefield
{
private:
    class ListNode{
    public:
        Robot* robot;
        ListNode* next;

        // INITIALIZE
        ListNode(Robot* r) : robot(r), next(nullptr){}
    };

    int BATTLEFIELD_NUM_OF_COLS_ = -1;
    int BATTLEFIELD_NUM_OF_ROWS_ = -1;
    int turns_ = 0;
    int Maxturns_ = -1;
    int numOfRobots_ = -1;
    Vector<Robot*> robots_;
    Vector<Vector<string>> battlefield_;
    ListNode* head_;
    ListNode* tail_;
    Queue<Robot*> queueWaitingRobots_;
    Queue<Robot*> queueDestroyedRobots_;
    Queue<pair<Robot*, int>> queueRevival_;

public:
    // PC & DC
    Battlefield():head_(nullptr), tail_(nullptr){}
    ~Battlefield(){
        clearTurnList();
        for (auto robot : robots_){
            delete robot;
        }
    }

    int BATTLEFIELD_NUM_OF_COLS() const { return BATTLEFIELD_NUM_OF_COLS_; }
    int BATTLEFIELD_NUM_OF_ROWS() const { return BATTLEFIELD_NUM_OF_ROWS_; }
    int MaxTurns()const {return Maxturns_;}
    int turns() { return turns_; }
    int numOfRobots() const { return numOfRobots_; }
    Vector<Robot*> robots() const { return robots_; }
    Vector<Vector<string>> battlefield() const { return battlefield_; }

    // LinkedList ---------------------------------------------------------------------------------------------
    void displayActiveRobots(){
        ListNode* activeRobot = head_;
        do {
            if (isRobotInVector(activeRobot->robot, robots_)){
            cout << activeRobot->robot->id() << " ";
            }
            activeRobot = activeRobot->next;
        } while (activeRobot != head_);
    }

    void displayRobotsTotalKills(){
        ListNode* robotTotalKills = head_;

        // FOR REMAINING ROBOTS
        cout << "\nNumber of lives of remaining robots: \n";
        do {
            if (isRobotInVector(robotTotalKills->robot, robots_)){
                cout << "Robot " << robotTotalKills->robot->id() << " with " << robotTotalKills->robot->numOfKills() << " total kills.\n";
            }
            robotTotalKills = robotTotalKills->next;
        } while (robotTotalKills != head_);

        // FOR DEAD ROBOTS
        cout << "\nNumber of lives of dead robots: \n";
        do {
            if (!isRobotInVector(robotTotalKills->robot, robots_)){
                cout << "Robot " << robotTotalKills->robot->id() << " with " << robotTotalKills->robot->numOfKills() << " total kills.\n";
            }
            robotTotalKills = robotTotalKills->next;
        } while (robotTotalKills != head_);
    }

    void clearTurnList(){
        ListNode* current = head_;
        while (current != nullptr) {
            ListNode* temp = current;
            current = current->next;
            delete temp->robot;
            delete temp;
            if (current == head_){
                break;
            }
        }
        head_ = tail_ = nullptr;
    }

    void addRobotToTurnList(Robot* robot){
        ListNode* newNode = new ListNode(robot);
        if (tail_ == nullptr){
            head_ = tail_ = newNode;
            tail_->next = head_; // CIRCULAR LINKED LIST
        } else {
            tail_->next = newNode;
            tail_ = newNode;
            tail_->next = head_; // CIRCULAR LINKED LIST
        }
    }

    void replaceRobotInTurnList(Robot* oldRobot, Robot* newRobot) {
        ListNode* current = head_;
        do {
            if (current->robot == oldRobot) {
                current->robot = newRobot;
                break;
            }
            current = current->next;
        } while (current != head_);
    }

    void readFile(string filename){
        string strIn;
        string robotType = "";
        string robotName = "";
        string robotId = "";
        string strX = "";
        string strY = "";
        int x = -1;
        int y = -1;
        int randomX = -1;
        int randomY= -1;

        ifstream inFile;
        inFile.open(filename);
        if (!inFile)
        {
            cout<< "Cannot open the file name: " << filename << endl;
            exit(1);
        }

        inFile >> strIn;
        inFile >> strIn;
        inFile >> strIn;

        inFile >> BATTLEFIELD_NUM_OF_COLS_;
        inFile >> BATTLEFIELD_NUM_OF_ROWS_;
        cout << "M by N: " << BATTLEFIELD_NUM_OF_COLS_ << " " << BATTLEFIELD_NUM_OF_ROWS_ << "\n";
        inFile >> strIn;
        inFile >> Maxturns_;
        cout << "turns: " << Maxturns_ << endl;
        inFile >> strIn;
        inFile >> numOfRobots_;

        cout << "robots: " << numOfRobots_ << endl;
        for (int i=0; i<numOfRobots_; ++i)
        {
            inFile >> robotType;
            inFile >> robotName;
            inFile >> strX;
            inFile >> strY;
            cout << robotType << " " << robotName << " "
                 << strX << " " << strY << "\n" ;

            size_t underscorePos = robotName.find('_');
            if (underscorePos != string::npos)
            {
                robotId = robotName.substr(0, underscorePos);
            }

            if( strX == "random" )
            {
                x = rand()% BATTLEFIELD_NUM_OF_COLS_;
                cout << robotId << " randomX: " << x << endl;
            }
            else
            {
                x = stoi (strX);
            }

            if( strY == "random")
            {
                y = rand()% BATTLEFIELD_NUM_OF_ROWS_;
                cout << robotId << " randomY: " << y << endl;
            }
            else
            {
                y = stoi (strY);
            }

            // PURPOSE: LINKED LIST
            Robot* newRobot = nullptr;

            if (robotType == "RoboCop")
            {
                newRobot = new RoboCop(robotId, x, y);
            }
            else if (robotType == "Terminator")
            {
                newRobot = new Terminator(robotId, x, y);
            }
            else if (robotType == "TerminatorRoboCop")
            {
                newRobot = new TerminatorRoboCop(robotId, x, y);
            }
            else if (robotType == "BlueThunder")
            {
                newRobot = new BlueThunder(robotId, x, y);
            }
            else if (robotType == "Madbot")
            {
                newRobot = new Madbot(robotId, x, y);
            }
            else if (robotType == "RoboTank")
            {
                newRobot = new RoboTank(robotId, x, y);
            }
            else if (robotType == "UltimateRobot")
            {
                newRobot = new UltimateRobot(robotId, x, y);
            }
            else if (robotType == "RobotBomber")
            {
                newRobot = new RobotBomber(robotId, x, y);
            }
            else if (robotType == "RobotLaser")
            {
                newRobot = new RobotLaser(robotId, x, y);
            }
            else if (robotType == "RobotTeleport")
            {
                newRobot = new RobotTeleport(robotId, x, y);
            }

            if (newRobot){
                newRobot->setOriginalIndex(i);
                robots_.push_back(newRobot);
                addRobotToTurnList(newRobot);
            }
        }
        inFile.close();

        battlefield_.resize(BATTLEFIELD_NUM_OF_ROWS_);
        for (int i=0; i<battlefield_.size(); ++i)
        {
            battlefield_[i].resize(BATTLEFIELD_NUM_OF_COLS_);
        }

        for (int i=0; i<battlefield_.size(); ++i)
        {
            for(int j=0; j<battlefield_[i].size(); ++j)
            {
                battlefield_[i][j] = "";
            }
        }

        placeRobots();
    }

    void placeRobots(){
        for (int i=0; i<battlefield_.size(); ++i)
        {
            for(int j=0; j<battlefield_[i].size(); ++j)
            {
                battlefield_[i][j] = "";
            }
        }

        for (int i=0; i<robots_.size(); ++i)
        {
            if (robots_[i]->y() < battlefield_.size() && robots_[i]->x() < battlefield_[0].size())
            {
                battlefield_[robots_[i]->y()] [robots_[i]->x()] = robots_[i]->id();
            }
            else
            {
                cout << "Error message: Invalid location for the robot " <<
                robots_[i]->id() << endl;
                exit(1);
            }
        }
    }

    void eraseRobotById(string id){
        for (int i=0; i<robots_.size(); ++i)
        {
            if (robots_[i]->id() == id){
                cout << "Kill robot " << *robots_[i] << endl;

                if (robots_[i]->numOfLives() != 0){
                    queueWaitingRobots_.enqueue(robots_[i]);
                    queueRevival_.enqueue(make_pair(robots_[i], turns_));
                } else {
                    queueDestroyedRobots_.enqueue(robots_[i]);
                }
                robots_.erase(robots_.begin() + i);
                break; // after loop
            }
        }
        placeRobots();
    }

    void pushBackRobot(Robot* robot){
        cout << "Push back robot" << *robot << endl;
        robots_.push_back(robot);
        placeRobots();
    }

    void displayBattlefield() const{
        cout << "    ";
        for (int j = 0; j<battlefield_[0].size(); ++j)
        {
            cout << " " << right << setfill('0') << setw(2) << j << "  ";
        }
        cout << endl;

        for (int i = 0; i < battlefield_.size(); ++i)
        {
            cout << "    ";
            for (int j = 0; j < battlefield_[0].size(); ++j)
                cout << "+----";
            cout << "+" << endl;

            cout << "  " << right << setfill('0') << setw(2) << i;

            for (int j = 0; j < battlefield_[0].size(); ++j)
            {
                if( battlefield_[i][j] == "" )
                {
                    cout << "|" << "    ";
                }
                else
                {
                    cout << "|" << left << setfill(' ') << setw(4) << battlefield_[i][j];
                }
            }
            cout << "|" << endl;
        }

        cout << "    ";
        for (int j = 0; j < battlefield_[0].size(); ++j)
             cout << "+----";
        cout << "+" << endl;
    }

    // ValidityCheck ---------------------------------------------------------------------------------------------
    bool isOccupied(int x, int y){
        for (const auto& robot : robots_){
            if(robot -> x() == x && robot->y() == y){
                return true;
            }
        }
        return false;
    }

    bool isRobotInVector(Robot* robot, const Vector<Robot*>& robotVector) {
        for (int i = 0; i < robotVector.size(); ++i) {
            if (robot == robotVector[i]) {
                return true;
            }
        }
        return false;
    }

    // Queue ---------------------------------------------------------------------------------------------
    bool isInQueue(Robot* robot, Queue<Robot*> queue){
        Queue<Robot*> tempQueue = queue;
        while (!tempQueue.empty()){
            if (tempQueue.front() == robot){
                return true;
            }
            tempQueue.dequeue();
        }
        return false;
    }

    void WaitingRobots(){
        Queue<Robot*> tempQueue;
        cout << "Waiting Robots Queue: ";

        if (queueWaitingRobots_.empty()){
            cout << "empty";
        }

        while (!queueWaitingRobots_.empty()){
            Robot* robot = queueWaitingRobots_.front();
            cout << robot->id() << " ";
            tempQueue.enqueue(robot);
            queueWaitingRobots_.dequeue();
        }

        while (!tempQueue.empty()){ // Restore the robots back to the original queue
            queueWaitingRobots_.enqueue(tempQueue.front());
            tempQueue.dequeue();
        }
    }

    void DestroyedRobots(){
        Queue<Robot*> tempQueue;
        cout << "Destroyed Robots Queue: ";

        if (queueDestroyedRobots_.empty()){
            cout << "empty";
        }

        while (!queueDestroyedRobots_.empty()){
            Robot* robot = queueDestroyedRobots_.front();
            cout << robot->id() << " ";
            tempQueue.enqueue(robot);
            queueDestroyedRobots_.dequeue();
        }

        while (!tempQueue.empty()){ // Restore the robots back to the original queue
            queueDestroyedRobots_.enqueue(tempQueue.front());
            tempQueue.dequeue();
        }

    }

    void reviveRobots() {
        bool revivedThisRound = false;

        if (!revivedThisRound && !queueRevival_.empty()) {
            pair<Robot*, int> robotTurnPair = queueRevival_.front();
            Robot* robot = robotTurnPair.first;
            int deathTurn = robotTurnPair.second;

            if (turns_ - deathTurn >= 1) { // Check if two turns have passed since the robot died
                queueRevival_.dequeue();
                if (!queueWaitingRobots_.empty()){
                    queueWaitingRobots_.dequeue();
                }

                int originalIndex = robot->originalIndex();
                int x = robot->x();
                int y = robot->y();
                if (isOccupied(x, y)) {
                    do {
                        x = rand() % BATTLEFIELD_NUM_OF_COLS_;
                        y = rand() % BATTLEFIELD_NUM_OF_ROWS_;
                    } while (isOccupied(x, y));
                }
                robot->setX(x);
                robot->setY(y);
                cout << robot->id();

                robots_.push_back(robot);

                int i = robots_.size()-1;
                while (i > 0 && robots_[i]->originalIndex() < robots_[i-1]->originalIndex()){
                    swap(robots_[i], robots_[i-1]);
                    i--;
                }

                setRobotBattlefieldPointers();
                updateBattlefield();
                revivedThisRound = true;
            }
        } else {
            cout << "empty";
        }
    }

    // DataUpdate ---------------------------------------------------------------------------------------------
    int getRobotIndexById(const string& id) const {
        for (size_t i = 0; i < robots_.size(); ++i) {
            if (robots_[i]->id() == id) {
                return i;
            }
        }
        return -1;
    }

    void setRobotBattlefieldPointers(){
        for(auto robot: robots_){
            robot->setBattlefield(this);
        }
    }

    void setRobotAt(size_t index, Robot* newRobot) {
        if (index < robots_.size()) {
            robots_[index] = newRobot;
        }
    }

    void updateBattlefield(){
        for (int i=0; i<battlefield_.size(); ++i){
            for (int j=0; j < battlefield_[i].size(); ++j){
                battlefield_[i][j] = " ";
            }
        }
        for (int i=0; i< robots_.size(); ++i){
            if (robots_[i]->y() < battlefield_.size() &&
                robots_[i]->x() < battlefield_[0].size()){
                    battlefield_[robots_[i]->y()][robots_[i]->x()] = robots_[i]->id();
                }
        }
    }

    // RobotActions ---------------------------------------------------------------------------------------------
    void runRobotActions(){
        ListNode* currentNode = head_;
        string lastRobotId = "";

        while (turns_ < Maxturns_ && robots_.size() > 1){
            // CONDITIONS TO PROCEED
            if (currentNode == nullptr){
                currentNode = head_;
            }
            bool validNode = false;
            for (int i = 0; i < robots_.size(); ++i) {
                if (currentNode->robot == robots_[i]) {
                    validNode = true;
                    break;
                }
            }
            if (!validNode){
                currentNode = currentNode->next;
                continue;
            }

            // NOTATIONS FOR TURNS
            cout << "......................................................"<< endl;
            cout << "Turn: " << turns_+1 << endl;

            // REVIVE ROBOT
            cout << "Robot revived in this turn: ";
            reviveRobots();
            cout << "\n";

            // DISPLAY ACTIVE ROBOTS
            cout << "Tracking actions of robots on the battlefield: ";
            displayActiveRobots();
            cout <<"using linked list.\n";

            // DISPLAY NUMBER OF LIVES
            //cout << "Number of lives: " << currentNode->robot->numOfLives() ;
            //cout << "\n\n";

            // ROBOTS ACTIONS
            cout << "\n" << *currentNode->robot << " actions: \n";
            currentNode->robot->actions();
            cout << "\n";

            // PLAYING NEXT
            ListNode* nextNode = currentNode ->next;
            while (nextNode != nullptr && !isRobotInVector(nextNode->robot, robots_)){
                nextNode = nextNode->next;
            }
            if (nextNode != nullptr){
                cout << "Next Robot Playing: " << nextNode->robot->id() << "\n";
            }

            // WAITING ROBOTS
            WaitingRobots();
            cout << "\n";

            // DESTROYED ROBOTS
            DestroyedRobots();
            cout << "\n";

            // DISPLAY BATTLEFIELD
            cout << "Display Battlefield \n\n";
            displayBattlefield();

            // UPDATE FOR NEXT LOOP
            lastRobotId = currentNode->robot->id();
            currentNode = currentNode->next;
            turns_++;

            // CONTINUATION CONDITION
            if (robots_.size()<= 1){
                break;
            }

        }

        cout << "......................................................\n";
        if (robots_.size() <= 1){
            cout << "Game over: One robot survivor!\n";
        } else if (turns_ >= Maxturns_){
            cout << "Game over: Maximum number of turns reached!\n\n";
        }

        cout << "Remaining robots on the battlefield: ";
        displayActiveRobots();
        cout << "\n";

        DestroyedRobots();
        cout << "\n\n";

        cout << "Displaying total kills... \n";
        displayRobotsTotalKills();

        // REMAINING ROBOTS
        cout << "\nDisplaying the final battlefield... \n\n";
        displayBattlefield();
    }
};

int main()
{
    // OUTPUT FILE
    ofstream outFile("output2a.txt");
    if (!outFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }
    streambuf *consoleBuffer = cout.rdbuf(); // Save original cout buffer
    stringstream ss;
    cout.rdbuf(ss.rdbuf()); // Redirect cout to stringstream

    // FIXED
    srand(1211109038);
    Battlefield battlefield;

    string filename = "fileinput2a.txt";
    cout << "Read " << filename << ":\n";
    battlefield.readFile(filename);
    cout << endl;

    cout << "Battlefield Display \n";
    battlefield.setRobotBattlefieldPointers();
    battlefield.displayBattlefield();

    cout << "\n";
    battlefield.runRobotActions();

    string consoleOutput = ss.str();
    cout.rdbuf(consoleBuffer); // Restore cout buffer
    cout << consoleOutput; // Output to console
    outFile << consoleOutput; // Output to file
    outFile.close();

    // HAHAHAHAHAHAHAHAHAHA
    exit(0);
}

// RoboCop ---------------------------------------------------------------------------------------------
void RoboCop::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    TerminatorRoboCop* newRobot = new TerminatorRoboCop(id_,robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SET A NEW ID
    std::ostringstream oss;
    oss << "TR" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void RoboCop::actionLook() {
    cout << "\nPerforming look action... \n";
    minX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    maxX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    minY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    maxY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    cout << "    ";
    for (int j = minX; j<= maxX ; ++j){
        cout << " " << right << setfill('0') << setw(2) << j << "  ";
    }

    cout << endl;

    for (int i = minY; i <= maxY; ++i){
        cout << "    ";
        for (int j = minX; j <= maxX; ++j){cout << "+----";}
        cout << "+" << endl;
        cout << "  " << right << setfill('0') << setw(2) << i;

        for (int j = minX; j <= maxX; ++j){
            if( battlefield->battlefield()[i][j] == "" ){cout << "|" << "    ";}
            else{cout << "|" << left << setfill(' ') << setw(4) << battlefield->battlefield()[i][j];}
        }
        cout << "|" << endl;
    }

    cout << "    ";
    for (int j = minX; j <= maxX; ++j){cout << "+----";}
    cout << "+" << endl;
}

void RoboCop::actionMove(){
    cout << "\nPerforming move action...\n";

    const int directions[8][2] = {
        {-1,  1}, // UP_LEFT
        { 0,  1}, // UP
        { 1,  1}, // UP_RIGHT
        {-1,  0}, // LEFT
        { 1,  0}, // RIGHT
        {-1, -1}, // DOWN_LEFT
        { 0, -1}, // DOWN
        { 1, -1}  // DOWN_RIGHT
    };

    bool moved = false;

    while (!moved) {
        tempX = robotPositionX;
        tempY = robotPositionY;

        srand(1211109038 + attempts + count);
        int randomIndex = rand() % 8;

        tempX += directions[randomIndex][0];
        tempY += directions[randomIndex][1];

        if (tempX >= 0 && tempX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
            tempY >= 0 && tempY < battlefield->BATTLEFIELD_NUM_OF_ROWS() &&
            !battlefield->isOccupied(tempX, tempY) &&
            (tempX != robotPositionX || tempY != robotPositionY)) {
            setX(tempX);
            setY(tempY);
            battlefield->updateBattlefield();
            cout << this->id() << " has moved to " << x() << " , " << y() << endl;
            moved = true;
            break;
        }

        attempts++;
        if (attempts >= 8){
            for (int i=0; i<8; ++i){
                tempX = robotPositionX + directions[i][0];
                tempY = robotPositionY + directions[i][1];

                if (tempX >= 0 && tempX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
                    tempY >= 0 && tempY < battlefield->BATTLEFIELD_NUM_OF_ROWS() &&
                    !battlefield->isOccupied(tempX, tempY) &&
                    (tempX != robotPositionX || tempY != robotPositionY)) {
                    setX(tempX);
                    setY(tempY);
                    battlefield->updateBattlefield();
                    cout << this->id() << " has moved to " << x() << " , " << y() << endl;
                    moved = true;
                    break;
                }
            }
            break;
        }
    }
    tempX = 0;
    tempY = 0;
}

void RoboCop:: actionFire(){
    cout << "\nPerforming fire action...\n";
    int shots = 3;

    while (shots > 0 && exists()){
        distX = 0;
        distY = 0;

        while(distance < 1 || distance > maxDistance ||
              targetFireX < 0 || targetFireX >= battlefield->BATTLEFIELD_NUM_OF_COLS() ||
              targetFireY < 0 || targetFireY >= battlefield->BATTLEFIELD_NUM_OF_ROWS())
        {
            distX = rand()%21-10;
            distY = rand()%21-10;
            distance = abs(distX) + abs(distY);
            targetFireX = distX + robotPositionX;
            targetFireY = distY + robotPositionY;
        }

        cout << "Fire at (" << targetFireX << ", " << targetFireY << "). ";

        if (battlefield->isOccupied(targetFireX, targetFireY)){
            string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];
            for(int i=0; i<battlefield->robots().size();++i){
                if(battlefield->robots()[i]->id()==robotKilledId){
                    battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                    battlefield->eraseRobotById(robotKilledId);
                }
            }
            this->addNumOfKills();
        } else {
            cout << "No kill. \n";
        }
        distance = 0;
        shots--;
    }
}

void RoboCop::actions(){
    actionLook();
    actionMove();
    actionFire();
    if (!exists()){return;}
}

// Terminator ---------------------------------------------------------------------------------------------
void Terminator::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    TerminatorRoboCop* newRobot = new TerminatorRoboCop(id_, robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SETTING A NEW ID
    std::ostringstream oss;
    oss << "TR" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void Terminator::actionLook(){
    cout << "\nPerforming look action... \n";
    minX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    maxX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    minY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    maxY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    cout << "    ";
    for (int j = minX; j<= maxX ; ++j){
        cout << " " << right << setfill('0') << setw(2) << j << "  ";
    }

    cout << endl;

    for (int i = minY; i <= maxY; ++i){
        cout << "    ";
        for (int j = minX; j <= maxX; ++j){cout << "+----";}
        cout << "+" << endl;
        cout << "  " << right << setfill('0') << setw(2) << i;

        for (int j = minX; j <= maxX; ++j){
            if( battlefield->battlefield()[i][j] == "" ){cout << "|" << "    ";}
            else{cout << "|" << left << setfill(' ') << setw(4) << battlefield->battlefield()[i][j];}
        }
        cout << "|" << endl;
    }

    cout << "    ";
    for (int j = minX; j <= maxX; ++j){cout << "+----";}
    cout << "+" << endl;
}

void Terminator::actionMove(){
    cout << "\nPerforming move action...\n";
    bool robotKilled = false;
    bool Terminated = false;

    int minRangeX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    int maxRangeX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    int minRangeY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    int maxRangeY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    for (int j = minRangeX; j<= maxRangeX ; ++j){
        int targetStepX = j;
        for(int i = minRangeY; i<=maxRangeY; ++i){
            int targetStepY = i;
            if(battlefield->isOccupied(targetStepX,targetStepY) &&
            (targetStepX != robotPositionX || targetStepY != robotPositionY))
            {
                // TERMINATE
                string robotKilledId = battlefield->battlefield()[targetStepY][targetStepX];
                for(int i=0; i<battlefield->robots().size();++i){
                    if(battlefield->robots()[i]->id()==robotKilledId){
                        //cout << battlefield->robots()[i]->id() << " terminated!" << endl;
                        battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                        battlefield->eraseRobotById(robotKilledId);
                        break; // inner loop
                    }
                }

                Robot::setX(targetStepX);
                Robot::setY(targetStepY);
                battlefield->updateBattlefield();
                addNumOfKills();
                robotKilled = true;

                // break the outer loop
                Terminated = true;
                break;
            }
        }
        if(Terminated) break;
    }

    if(!robotKilled){
        srand(1211109038);
        int randomX, randomY;
        do {
            randomX = minRangeX + rand()%(maxRangeX - minRangeX + 1);
            randomY = minRangeY + rand()%(maxRangeY - minRangeY + 1);

        } while (randomX == robotPositionX && randomY== robotPositionY);

        Robot::setX(randomX);
        Robot::setY(randomY);
        battlefield->updateBattlefield();
        cout << this->id() << " has moved to " << x() << " , " << y() << ". No kill. \n";
        cout << this->id() << " total number of kills: " << this->numOfKills() << endl;
    }
}

void Terminator::actions(){
    actionLook();
    actionMove();
    if (!exists()){return;}
}

// TerminatorRoboCop ---------------------------------------------------------------------------------------------
void TerminatorRoboCop::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    UltimateRobot* newRobot = new UltimateRobot(id_, robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SETTING A NEW ID
    std::ostringstream oss;
    oss << "UR" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void TerminatorRoboCop::actionLook(){
    cout << "\nPerforming look action... \n";
    minX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    maxX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    minY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    maxY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    cout << "    ";
    for (int j = minX; j<= maxX ; ++j){
        cout << " " << right << setfill('0') << setw(2) << j << "  ";
    }

    cout << endl;

    for (int i = minY; i <= maxY; ++i){
        cout << "    ";
        for (int j = minX; j <= maxX; ++j){cout << "+----";}
        cout << "+" << endl;
        cout << "  " << right << setfill('0') << setw(2) << i;

        for (int j = minX; j <= maxX; ++j){
            if( battlefield->battlefield()[i][j] == "" ){cout << "|" << "    ";}
            else{cout << "|" << left << setfill(' ') << setw(4) << battlefield->battlefield()[i][j];}
        }
        cout << "|" << endl;
    }

    cout << "    ";
    for (int j = minX; j <= maxX; ++j){cout << "+----";}
    cout << "+" << endl;
}

void TerminatorRoboCop::actionFire(){
    cout << "\nPerforming fire action...\n";
    int shots = 3;
    srand(1211109038); // CANNOT CHANGE
    while (shots > 0 && exists()){
        distX = 0;
        distY = 0;

        while(distance < 1 || distance > maxDistance ||
              targetFireX < 0 || targetFireX >= battlefield->BATTLEFIELD_NUM_OF_COLS() ||
              targetFireY < 0 || targetFireY >= battlefield->BATTLEFIELD_NUM_OF_ROWS())
        {
            distX = rand()%21-10;
            distY = rand()%21-10;
            distance = abs(distX) + abs(distY);
            targetFireX = distX + robotPositionX;
            targetFireY = distY + robotPositionY;
        }

        cout << "Fire at (" << targetFireX << ", " << targetFireY << "). ";

        if (battlefield->isOccupied(targetFireX, targetFireY)){
            cout << "Nice shot! ";
            string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

            for(int i=0; i<battlefield->robots().size();++i){
                if(battlefield->robots()[i]->id()==robotKilledId){
                    battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                    battlefield->eraseRobotById(robotKilledId);
                }
            }
            this->addNumOfKills();
        } else {
            cout << "No kill. \n";
            //cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
        }

        distance = 0;
        shots--;
    }
}

void TerminatorRoboCop::actionStep(){
    cout << "\nPerforming step action...\n";
    bool robotKilled = false;
    bool Terminated = false;

    // Define the search range for the 3x3 window around the Terminator
    int minRangeX = (robotPositionX - 1) > 0 ? (robotPositionX - 1) : 0;
    int maxRangeX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS() - 1);
    int minRangeY = (robotPositionY - 1) > 0 ? (robotPositionY - 1) : 0;
    int maxRangeY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1);

    // Iterate over each cell in the 3x3 window
    for (int j = minRangeX; j <= maxRangeX; ++j) {
        int targetStepX = j;
        for (int i = minRangeY; i <= maxRangeY; ++i) {
            int targetStepY = i;
            // Check if the cell is occupied by another robot
            if (battlefield->isOccupied(targetStepX, targetStepY) &&
                (targetStepX != robotPositionX || targetStepY != robotPositionY)) {

                // Terminate the robot in the occupied cell
                string robotKilledId = battlefield->battlefield()[targetStepY][targetStepX];
                for (int i = 0; i < battlefield->robots().size(); ++i) {
                    if (battlefield->robots()[i]->id() == robotKilledId) {
                        cout << "Step and ";
                        battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                        battlefield->eraseRobotById(robotKilledId);
                        break; // inner loop
                    }
                }

                // Move step-by-step to the new position
                int stepX = robotPositionX;
                int stepY = robotPositionY;

                while (stepX != targetStepX || stepY != targetStepY) {
                    if (stepX < targetStepX) {
                        stepX++;
                    } else if (stepX > targetStepX) {
                        stepX--;
                    }
                    if (battlefield->isOccupied(stepX, stepY) && !(stepX == robotPositionX && stepY == robotPositionY)) {
                        string robotInPathId = battlefield->battlefield()[stepY][stepX];
                        for (int i = 0; i < battlefield->robots().size(); ++i) {
                            if (battlefield->robots()[i]->id() == robotInPathId) {
                                cout << battlefield->robots()[i]->id() << " step and ";
                                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                                battlefield->eraseRobotById(robotInPathId);
                                break;
                            }
                        }
                    }

                    if (stepY < targetStepY) {
                        stepY++;
                    } else if (stepY > targetStepY) {
                        stepY--;
                    }
                    if (battlefield->isOccupied(stepX, stepY) && !(stepX == robotPositionX && stepY == robotPositionY)) {
                        string robotInPathId = battlefield->battlefield()[stepY][stepX];
                        for (int i = 0; i < battlefield->robots().size(); ++i) {
                            if (battlefield->robots()[i]->id() == robotInPathId) {
                                cout << battlefield->robots()[i]->id() << " step and ";
                                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                                battlefield->eraseRobotById(robotInPathId);
                                break;
                            }
                        }
                    }

                    // Update the Terminator's position step-by-step
                    Robot::setX(stepX);
                    Robot::setY(stepY);
                    battlefield->updateBattlefield();
                    //cout << this->id() << " moved to " << x() << " , " << y() << endl;
                }
                cout << this->id() << " has moved to " << this->x() << " , " << this->y() << endl;
                addNumOfKills();
                robotKilled = true;

                // Break out of the outer loop as the Terminator can only move once per turn
                Terminated = true;
                break;
            }
        }
        if (Terminated) break;
    }

    // If no robots were killed, perform a random move
    if (!robotKilled) {
        srand(1211109038);
        int randomX, randomY;
        do {
            randomX = minRangeX + rand() % (maxRangeX - minRangeX + 1);
            randomY = minRangeY + rand() % (maxRangeY - minRangeY + 1);

        } while (randomX == robotPositionX && randomY == robotPositionY);

        Robot::setX(randomX);
        Robot::setY(randomY);
        battlefield->updateBattlefield();
        cout << this->id() << " has moved to " << this->x() << " , " << this->y() << endl;
        cout << "No robot kill." << endl;
        cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";

    }
}

void TerminatorRoboCop::actions(){
    actionLook();
    actionFire();
    actionStep();
}

// BlueThunder ---------------------------------------------------------------------------------------------
void BlueThunder::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    Madbot* newRobot = new Madbot(id_, robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SETTING A NEW ID
    std::ostringstream oss;
    oss << "MB" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void BlueThunder::actionFire(){
    if (shots == 8){
        targetFireX = robotPositionX;
        targetFireY = robotPositionY - 1;

    } else if (shots == 7){
        targetFireX = robotPositionX + 1;
        targetFireY = robotPositionY - 1;

    } else if (shots==6){
        targetFireX = robotPositionX + 1;
        targetFireY = robotPositionY;

    } else if (shots == 5){
        targetFireX = robotPositionX + 1;
        targetFireY = robotPositionY + 1;

    } else if (shots == 4 ){
        targetFireX = robotPositionX;
        targetFireY = robotPositionY + 1;

    } else if (shots == 3){
        targetFireX = robotPositionX - 1;
        targetFireY = robotPositionY + 1;

    } else if (shots == 2){
        targetFireX = robotPositionX - 1;
        targetFireY = robotPositionY;

    } else if (shots == 1){
        targetFireX = robotPositionX - 1;
        targetFireY = robotPositionY - 1;
    }

    if (targetFireX >= 0 && targetFireX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
            targetFireY >= 0 && targetFireY < battlefield->BATTLEFIELD_NUM_OF_ROWS())
    {
        cout << "Blue Thunder fires at (" << targetFireX << ", " << targetFireY << "). ";
    }

    if (battlefield->isOccupied(targetFireX, targetFireY)){
        string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

        for(int i=0; i<battlefield->robots().size();++i){
            if(battlefield->robots()[i]->id()==robotKilledId){
                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                battlefield->eraseRobotById(robotKilledId);
            }
        }
        this->addNumOfKills();
    } else {
        cout << "No kill. \n";
        cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
    }
    shots--;
}

void BlueThunder::actions(){
    if (shots <= 0){
        shots = 8; // reset
        actionFire();
        if (!exists()){return;}

    } else {
        actionFire();
        if (!exists()){return;}
    }
}

// MadBot ---------------------------------------------------------------------------------------------
void Madbot::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    RoboTank* newRobot = new RoboTank(id_, robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SETTING A NEW ID
    std::ostringstream oss;
    oss << "RT" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void Madbot::actionFire(){
    //srand(1211109038); // Seed the random number generator

    // Define the immediate neighbors using vector
    Vector<pair<int, int>> neighbors = {
        {robotPositionX - 1, robotPositionY - 1}, // Top-left
        {robotPositionX, robotPositionY - 1},     // Top
        {robotPositionX + 1, robotPositionY - 1}, // Top-right
        {robotPositionX - 1, robotPositionY},     // Left
        {robotPositionX + 1, robotPositionY},     // Right
        {robotPositionX - 1, robotPositionY + 1}, // Bottom-left
        {robotPositionX, robotPositionY + 1},     // Bottom
        {robotPositionX + 1, robotPositionY + 1}  // Bottom-right
    };

    // Choose a random neighbor to fire at
    int index = rand() % 8;
    targetFireX = neighbors[index].first;
    targetFireY = neighbors[index].second;

    // Ensure the target is within the grid boundaries
    if (targetFireX >= 0 && targetFireX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
        targetFireY >= 0 && targetFireY < battlefield->BATTLEFIELD_NUM_OF_ROWS()) {
        cout << "MadBot fires at (" << targetFireX << ", " << targetFireY << "). ";

        if (battlefield->isOccupied(targetFireX, targetFireY)){
            string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

            for(int i=0; i<battlefield->robots().size();++i){
                if(battlefield->robots()[i]->id()==robotKilledId){
                    // modify for the robots with more than 1 lives
                    battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                    battlefield->eraseRobotById(robotKilledId);
                }
            }
            this->addNumOfKills();

        } else {
            cout << "No kill.\n";
            cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
        }


    } else {
        cout << "MadBot fires but misses as target is out of grid bounds" << endl;
    }
}

void Madbot::actions(){
    actionFire();
    if (!exists()){return;}
}

// RoboTank ---------------------------------------------------------------------------------------------
void RoboTank::upgrade(){
    Robot* oldRobot = nullptr;
    string oldRobotId = this->id();
    int remainingLives = this->numOfLives();

    // SCANNING THROUGH THE VECTORS
    for(size_t i=0; i<battlefield->robots().size();++i){
        if(battlefield->robots()[i]->id()== oldRobotId){
            oldRobot = battlefield->robots()[i];
            break;
        }
    }

    UltimateRobot* newRobot = new UltimateRobot(id_, robotPositionX, robotPositionY);
    newRobot->setRobotName(robotName_);
    newRobot->setNumOfLives(remainingLives);

    // SETTING A NEW ID
    std::ostringstream oss;
    oss << "UR" << std::setw(2) << std::setfill('0') << newRobot->robotAutoIncrementInt();
    std::string newId = oss.str();
    newRobot->setId(newId);

    int currentIndex = battlefield->getRobotIndexById(id_);
    cout << oldRobot->id() << " upgraded from " << oldRobot->robotType() << " to " << newRobot->robotType() << " with new robot ID " << newRobot->id() << "\n";

    if (currentIndex != -1) {
        battlefield->setRobotAt(currentIndex, newRobot);
        battlefield->replaceRobotInTurnList(this, newRobot);
        battlefield->setRobotBattlefieldPointers();
        battlefield->placeRobots();
        isValid = false;
        delete oldRobot;
    }
}

void RoboTank::actionFire(){
    //srand(1211109038);

    while(distance < 1 ||
        targetFireX < 0 || targetFireX >= battlefield->BATTLEFIELD_NUM_OF_COLS() ||
        targetFireY < 0 || targetFireY >= battlefield->BATTLEFIELD_NUM_OF_ROWS())
    {
        distX = rand()%battlefield->BATTLEFIELD_NUM_OF_COLS()-robotPositionX;
        distY = rand()%battlefield->BATTLEFIELD_NUM_OF_ROWS()-robotPositionY;
        distance = abs(distX) + abs(distY);
        targetFireX = robotPositionX + distX;
        targetFireY = robotPositionY + distY;
    }

    cout << "Fire at (" << targetFireX << ", " << targetFireY << "). ";

    if (battlefield->isOccupied(targetFireX, targetFireY)){
        string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

        for(int i=0; i<battlefield->robots().size();++i){
            if(battlefield->robots()[i]->id()==robotKilledId){
                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                battlefield->eraseRobotById(robotKilledId);
            }
        }
        this->addNumOfKills();
    } else {
        cout << "No kill. \n";
        cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
    }
    distance = 0;
}

void RoboTank::actions(){
    actionFire();
    if (!exists()){return;}
}

// UltimateRobot ---------------------------------------------------------------------------------------------
void UltimateRobot::actionLook(){
    cout << "\nPerforming look action... \n";
    minX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    maxX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    minY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    maxY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    cout << "    ";
    for (int j = minX; j<= maxX ; ++j){
        cout << " " << right << setfill('0') << setw(2) << j << "  ";
    }

    cout << endl;

    for (int i = minY; i <= maxY; ++i){
        cout << "    ";
        for (int j = minX; j <= maxX; ++j){cout << "+----";}
        cout << "+" << endl;
        cout << "  " << right << setfill('0') << setw(2) << i;

        for (int j = minX; j <= maxX; ++j){
            if( battlefield->battlefield()[i][j] == "" ){cout << "|" << "    ";}
            else{cout << "|" << left << setfill(' ') << setw(4) << battlefield->battlefield()[i][j];}
        }
        cout << "|" << endl;
    }

    cout << "    ";
    for (int j = minX; j <= maxX; ++j){cout << "+----";}
    cout << "+" << endl;
}

void UltimateRobot::actionMove(){
    cout << "\nPerforming move action...\n";
    bool robotKilled = false;
    bool Terminated = false;

    int minRangeX = (robotPositionX - 1) > 0 ? (robotPositionX-1):0;
    int maxRangeX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS()-1);
    int minRangeY = (robotPositionY - 1) > 0 ? (robotPositionY-1):0;
    int maxRangeY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS()-1);

    for (int j = minRangeX; j<= maxRangeX ; ++j){
        int targetStepX = j;
        for(int i = minRangeY; i<=maxRangeY; ++i){
            int targetStepY = i;
            if(battlefield->isOccupied(targetStepX,targetStepY) &&
            (targetStepX != robotPositionX || targetStepY != robotPositionY))
            {
                // TERMINATE
                string robotKilledId = battlefield->battlefield()[targetStepY][targetStepX];
                for(int i=0; i<battlefield->robots().size();++i){
                    if(battlefield->robots()[i]->id()==robotKilledId){
                        cout << battlefield->robots()[i]->id() << " terminated!" << endl;
                        battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                        battlefield->eraseRobotById(robotKilledId);
                        break; // inner loop
                    }
                }

                Robot::setX(targetStepX);
                Robot::setY(targetStepY);
                battlefield->updateBattlefield();
                addNumOfKills();
                robotKilled = true;

                // break the outer loop
                Terminated = true;
                break;
            }
        }
        if(Terminated) break;
    }

    if(!robotKilled){
        srand(1211109038);
        int randomX, randomY;
        do {
            randomX = minRangeX + rand()%(maxRangeX - minRangeX + 1);
            randomY = minRangeY + rand()%(maxRangeY - minRangeY + 1);

        } while (randomX == robotPositionX && randomY== robotPositionY);

        Robot::setX(randomX);
        Robot::setY(randomY);
        battlefield->updateBattlefield();
        cout << this->id() << " has moved to " << x() << " , " << y() << endl;
        cout << "No kill. " << endl;
        cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
    }
}

void UltimateRobot::actionFire(){
    cout << "\nPerforming fire action...\n";
    int shots = 3;
    srand(1211109038); // CANNOT REMOVE
    while (shots > 0){
        distX = 0;
        distY = 0;

        while(distance < 1 ||
              targetFireX < 0 || targetFireX >= battlefield->BATTLEFIELD_NUM_OF_COLS() ||
              targetFireY < 0 || targetFireY >= battlefield->BATTLEFIELD_NUM_OF_ROWS())
        {
            distX = rand()%battlefield->BATTLEFIELD_NUM_OF_COLS()-robotPositionX;
            distY = rand()%battlefield->BATTLEFIELD_NUM_OF_ROWS()-robotPositionY;
            distance = abs(distX) + abs(distY);
            targetFireX = distX + robotPositionX;
            targetFireY = distY + robotPositionY;
        }

        cout << "Fire at (" << targetFireX << ", " << targetFireY << "). ";

        if (battlefield->isOccupied(targetFireX, targetFireY)){
            string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

            for(int i=0; i<battlefield->robots().size();++i){
                if(battlefield->robots()[i]->id()==robotKilledId){
                    battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                    battlefield->eraseRobotById(robotKilledId);
                }
            }
            this->addNumOfKills();
        } else {
            cout << "No kill. \n";
            cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
        }

        distance = 0;
        shots--;
    }
}

void UltimateRobot::actionStep(){
    cout << "\nPerforming step action...\n";
    bool robotKilled = false;
    bool Terminated = false;

    // Define the search range for the 3x3 window around the Terminator
    int minRangeX = (robotPositionX - 1) > 0 ? (robotPositionX - 1) : 0;
    int maxRangeX = (robotPositionX + 1) < battlefield->BATTLEFIELD_NUM_OF_COLS() - 1 ? (robotPositionX + 1) : (battlefield->BATTLEFIELD_NUM_OF_COLS() - 1);
    int minRangeY = (robotPositionY - 1) > 0 ? (robotPositionY - 1) : 0;
    int maxRangeY = (robotPositionY + 1) < battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1 ? (robotPositionY + 1) : (battlefield->BATTLEFIELD_NUM_OF_ROWS() - 1);

    // Iterate over each cell in the 3x3 window
    for (int j = minRangeX; j <= maxRangeX; ++j) {
        int targetStepX = j;
        for (int i = minRangeY; i <= maxRangeY; ++i) {
            int targetStepY = i;
            // Check if the cell is occupied by another robot
            if (battlefield->isOccupied(targetStepX, targetStepY) &&
                (targetStepX != robotPositionX || targetStepY != robotPositionY)) {

                // Terminate the robot in the occupied cell
                string robotKilledId = battlefield->battlefield()[targetStepY][targetStepX];
                for (int i = 0; i < battlefield->robots().size(); ++i) {
                    if (battlefield->robots()[i]->id() == robotKilledId) {
                        battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                        cout << "Step and ";
                        battlefield->eraseRobotById(robotKilledId);
                        break; // inner loop
                    }
                }

                // Move step-by-step to the new position
                int stepX = robotPositionX;
                int stepY = robotPositionY;

                while (stepX != targetStepX || stepY != targetStepY) {
                    if (stepX < targetStepX) {
                        stepX++;
                    } else if (stepX > targetStepX) {
                        stepX--;
                    }
                    if (battlefield->isOccupied(stepX, stepY) && !(stepX == robotPositionX && stepY == robotPositionY)) {
                        string robotInPathId = battlefield->battlefield()[stepY][stepX];
                        for (int i = 0; i < battlefield->robots().size(); ++i) {
                            if (battlefield->robots()[i]->id() == robotInPathId) {
                                // cout << battlefield->robots()[i]->id() << " stepped in path!" << endl;
                                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                                battlefield->eraseRobotById(robotInPathId);
                                break;
                            }
                        }
                    }

                    if (stepY < targetStepY) {
                        stepY++;
                    } else if (stepY > targetStepY) {
                        stepY--;
                    }
                    if (battlefield->isOccupied(stepX, stepY) && !(stepX == robotPositionX && stepY == robotPositionY)) {
                        string robotInPathId = battlefield->battlefield()[stepY][stepX];
                        for (int i = 0; i < battlefield->robots().size(); ++i) {
                            if (battlefield->robots()[i]->id() == robotInPathId) {
                                //cout << battlefield->robots()[i]->id() << " stepped in path!" << endl;
                                battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                                battlefield->eraseRobotById(robotInPathId);
                                break;
                            }
                        }
                    }

                    // Update the Terminator's position step-by-step
                    Robot::setX(stepX);
                    Robot::setY(stepY);
                    battlefield->updateBattlefield();
                    //cout << this->id() << " moved to " << x() << " , " << y() << endl;
                }
                addNumOfKills();
                robotKilled = true;

                // Break out of the outer loop as the Terminator can only move once per turn
                Terminated = true;
                break;
            }
        }
        if (Terminated) break;
    }

    // If no robots were killed, perform a random move
    if (!robotKilled) {
        srand(1211109038);
        int randomX, randomY;
        do {
            randomX = minRangeX + rand() % (maxRangeX - minRangeX + 1);
            randomY = minRangeY + rand() % (maxRangeY - minRangeY + 1);

        } while (randomX == robotPositionX && randomY == robotPositionY);

        Robot::setX(randomX);
        Robot::setY(randomY);
        battlefield->updateBattlefield();
        cout << "No kill. ";
        cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";

    }
    cout << this->id() << " has moved to " << this->x() << " , " << this->y() << endl;
}

void UltimateRobot::actions(){
    actionLook();
    actionMove();
    actionFire();
    actionStep();
}

// RobotBomber ---------------------------------------------------------------------------------------------
void RobotBomber::actionFire(){
    int shots = 1;
    bool isFirstShot = true;
    while (shots > 0) {
        distX = 0;
        distY = 0;

        while (distance < 1 ||
               targetFireX < 0 || targetFireX >= battlefield->BATTLEFIELD_NUM_OF_COLS() ||
               targetFireY < 0 || targetFireY >= battlefield->BATTLEFIELD_NUM_OF_ROWS()) {
            distX = rand() % battlefield->BATTLEFIELD_NUM_OF_COLS() - robotPositionX;
            distY = rand() % battlefield->BATTLEFIELD_NUM_OF_ROWS() - robotPositionY;
            distance = abs(distX) + abs(distY);
            targetFireX = distX + robotPositionX;
            targetFireY = distY + robotPositionY;
        }


        Vector<pair<int, int>> targets = {
            {targetFireX, targetFireY},
            {targetFireX - 1, targetFireY - 1}, // Top-left
            {targetFireX, targetFireY - 1},     // Top
            {targetFireX + 1, targetFireY - 1}, // Top-right
            {targetFireX - 1, targetFireY},     // Left
            {targetFireX + 1, targetFireY},     // Right
            {targetFireX - 1, targetFireY + 1}, // Bottom-left
            {targetFireX, targetFireY + 1},     // Bottom
            {targetFireX + 1, targetFireY + 1}  // Bottom-right
        };

        for (auto target : targets) {
            int fireX = target.first;
            int fireY = target.second;

            // Ensure the target is within battlefield bounds
            if (fireX >= 0 && fireX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
                fireY >= 0 && fireY < battlefield->BATTLEFIELD_NUM_OF_ROWS()) {

                if (isFirstShot) {
                    cout << "Bombed at (" << fireX << ", " << fireY << ")" << endl;
                    isFirstShot = false; // Set isFirstShot to false after the first shot
                } else {
                    cout << "Boom goes (" << fireX << ", " << fireY << ")" << endl;
                }

                if (battlefield->isOccupied(fireX, fireY)) {
                    cout << "Nice shot! ";
                    string robotKilledId = battlefield->battlefield()[fireY][fireX];

                    for (int i = 0; i < battlefield->robots().size(); ++i) {
                        if (battlefield->robots()[i]->id() == robotKilledId) {
                            // Modify for the robots with more than 1 life
                            battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                            battlefield->eraseRobotById(robotKilledId);
                        }
                    }
                    this->addNumOfKills();
                } else {
                    cout << "No kill. ";
                    cout << this->id() << " total number of kills: " << this->numOfKills() << "\n";
                }
            }
        }

        distance = 0;
        shots--;
    }
}

void RobotBomber::actions(){
    actionFire();
}

// RobotLaser ------------------------------------------------------------------------------------------
void RobotLaser::actionFire(){
    // Fire one shot to the right of the robot
    targetFireX = robotPositionX + 1;
    targetFireY = robotPositionY;

    if (targetFireX >= 0 && targetFireX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
            targetFireY >= 0 && targetFireY < battlefield->BATTLEFIELD_NUM_OF_ROWS())
        {
            cout << "RobotLaser fires at (" << targetFireX << ", " << targetFireY << "). ";
        }

    for (int i = targetFireX; i < battlefield->BATTLEFIELD_NUM_OF_COLS(); i++)
    {

        if (battlefield->isOccupied(targetFireX, targetFireY)){
            string robotKilledId = battlefield->battlefield()[targetFireY][targetFireX];

            for(int i=0; i<battlefield->robots().size();++i){
                if(battlefield->robots()[i]->id()==robotKilledId){
                    battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives()-1);
                    battlefield->eraseRobotById(robotKilledId);
                }
            }
            this->addNumOfKills();
        } else {
            cout << "\nNo kill. " ;
            cout << this->id() << " total number of kills: " << this->numOfKills();
        }

        targetFireX++;
    }
    cout << "\nuntil (" << targetFireX - 1 << ", " << targetFireY << ")" << endl;
}

void RobotLaser::actions(){
    actionFire();
}

// RobotTeleport ------------------------------------------------------------------------------------------
void RobotTeleport::actionMove(){
    //srand(time(0));
    int randomX = rand() % battlefield->BATTLEFIELD_NUM_OF_COLS();
    int randomY = rand() % battlefield->BATTLEFIELD_NUM_OF_ROWS();

    // Ensure the random position is not occupied
    while (battlefield->isOccupied(randomX, randomY)) {
        randomX = rand() % battlefield->BATTLEFIELD_NUM_OF_COLS();
        randomY = rand() % battlefield->BATTLEFIELD_NUM_OF_ROWS();
    }

    Robot::setX(randomX);
    Robot::setY(randomY);
    battlefield->updateBattlefield();
    cout << this->id() << " moved to (" << Robot::x() << ", " << Robot::y() << ")\n";
}

void RobotTeleport::actionFire(){
    // Define the immediate neighbors
    Vector<pair<int, int>> neighbors = {
        {robotPositionX - 1, robotPositionY - 1}, // Top-left
        {robotPositionX, robotPositionY - 1},     // Top
        {robotPositionX + 1, robotPositionY - 1}, // Top-right
        {robotPositionX - 1, robotPositionY},     // Left
        {robotPositionX + 1, robotPositionY},     // Right
        {robotPositionX - 1, robotPositionY + 1}, // Bottom-left
        {robotPositionX, robotPositionY + 1},     // Bottom
        {robotPositionX + 1, robotPositionY + 1}  // Bottom-right
    };

    // Fire at all neighbors
    for (auto& neighbor : neighbors) {
        int fireX = neighbor.first;
        int fireY = neighbor.second;

        if (fireX >= 0 && fireX < battlefield->BATTLEFIELD_NUM_OF_COLS() &&
            fireY >= 0 && fireY < battlefield->BATTLEFIELD_NUM_OF_ROWS()) {

            cout << "RobotTeleport bombs (" << fireX << ", " << fireY << ")\n";

            if (battlefield->isOccupied(fireX, fireY)) {
                cout << "Hit!\n";
                string robotKilledId = battlefield->battlefield()[fireY][fireX];

                for (int i = 0; i < battlefield->robots().size(); ++i) {
                    if (battlefield->robots()[i]->id() == robotKilledId) {
                        battlefield->robots()[i]->setNumOfLives(battlefield->robots()[i]->numOfLives() - 1);
                        battlefield->eraseRobotById(robotKilledId);
                    }
                }
                this->addNumOfKills();
            } else {
                cout << "No robots were killed. The total number of kills: " << this->numOfKills() << "\n";
            }
        }
    }
}

void RobotTeleport::actions(){
    actionMove();
    actionFire();
}
