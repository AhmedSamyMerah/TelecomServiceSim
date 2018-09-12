#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <ctime>
using namespace std;

class Call{
    private:
        //field variables of call class
        string phoneNumber;
        int duration;
    public:
        //constructor of the call class
        Call(string pNum, int sec){
            phoneNumber = pNum;
            duration = sec;
        }
    
    //getters for the fields of the call class
    string getNumber() {
        return phoneNumber;
    }
    int getDuration() {
        return duration;
    }     
};

class Customer{
    private:
        //fields of the customer class
        string customerName;
        float balance;
        
    protected:
        //vector that will hold the calls
        vector <Call*> vectCall;
        //protected fields for inheritance
        float monthlyFee;
        float chPerCall;
        float chPerMinute;
    public:
        //constructor of customer class
        Customer(string cName) {
        customerName = cName;
        balance = 0;
    }
        
    //getters and setters of the fields
    void setName(string name) {
        customerName = name;
    }
    string getName() {
        return customerName;
    }
    
    //methods to add a call to the vector
    void addCall(string number, int duration) {
        Call *call = new Call(number, duration);
        vectCall.push_back(call);
    }
    //method to get the call from the vector
    Call* getCall(int i){
        Call *call = vectCall[i];
        return call;
    }
    
    int getNumberOfCalls(){
        return vectCall.size();
    }
    void setBalance(float bal) {
        balance = bal;
    }
    float getBalance() {
        return balance;
    }
    //full virtual method
    virtual void computeBalance() = 0;
};


class RegularCustomer : public Customer {
public:

    RegularCustomer(string cName) :
    Customer(cName) {
        //initializing fields due to the different calculations involved 
        monthlyFee = 5;
        chPerCall = 0.90;
        chPerMinute = 0;
    }
    
    //overriding the virtual class
    void computeBalance() {
        float currentBalance = currentBalance = monthlyFee + (chPerCall * vectCall.size());
        setBalance(currentBalance);
    }
};

class PremiumCustomer : public Customer {
public:
    PremiumCustomer(string cName) : Customer(cName) {
        //initializing fields due to the different calculations involved 
        monthlyFee = 30;
        chPerCall = 0.05;
        chPerMinute = 0.01;
    }
    
    //overriding the virtual class
    void computeBalance() {
        int numSeconds = 0;
        Call *call;
        
        //loop to get the duration of each call as we take into account the duration of the call to compute a premium customer bill
        for(int i=0; i < vectCall.size()-1; i++){
            call = vectCall[i];
            numSeconds += call->getDuration();
        }
        float currentBalance = monthlyFee + (chPerCall * vectCall.size()) + (chPerMinute * (numSeconds/60));
        setBalance(currentBalance);
    }
};


class Simulation {
private:
    //fields of the simulation class
    //vectors that holds the types of customers
    vector<RegularCustomer*> regVect;
    vector<PremiumCustomer*> preVect;
    RegularCustomer *rCust;
    PremiumCustomer *pCust;
    
    //Array used to generate names
    char alphabet[52] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    //fields that will hold the balances (average) of both types of customers
    float averagePreBalance;
    float averageBalance;
    
public:
    //constructor of the sim class
    Simulation() {
        srand(time(NULL));

        // generator of number of regular customers and premium.
        int numReg = rand() % (400 - 200 + 1) + 200; 
        int numPre = rand() % (400 - 200 + 1) + 200;
        
        // create regular customers
        for (int regCust = 0; regCust < numReg; regCust++) {
            
            rCust = new RegularCustomer(genRandomName());

            // adding to vector of calls objects
            for (int i = 0; i < genRandomCalls(); i++) {
                rCust->addCall(genPhoneNumber(), genDuration());
            }

            // add regular customer to vector of customers.
            regVect.push_back(rCust);
        }
        
        // create premium customer
        for (int preCust = 0; preCust < numPre; preCust++) {
            
            pCust = new PremiumCustomer(genRandomName());

            // adding to vector of calls objects
            for (int i = 0; i < genRandomCalls(); i++) {
                pCust->addCall(genPhoneNumber(), genDuration() );
            }

            // add premium customer to vector of customers
            preVect.push_back(pCust);
        }
    }

    string genRandomName() {

        //variables that will hold the names
        string fName;
        string lName;

        //for loop to get first name
        for (int i = 0; i < (rand() % (6 - 4 + 1) + 4); i++) {
            fName += alphabet[rand() % (51 - 0 + 1) + 0];
        }

        //for loop to get last name
        for (int i = 0; i < (rand() % (6 - 4 + 1) + 4); i++) {
            lName += alphabet[rand() % (51 - 0 + 1) + 0];
        }

        //adding the strings to get a full name
        string fullName = (fName + " " + lName);
        return fullName;

    }

    int genRandomCalls() {
        //generating random number of calls between 10 and 100
        int randomCalls = (rand() % (100 - 10 + 1) + 10);
        return randomCalls;
    }

    string genPhoneNumber() {
        //generating a random US 10 digit number
        string pNumber;
        char nums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 0; i<sizeof (nums); i++) {
            pNumber += nums[(rand() % (9 - 0 + 1) + 0)];
        }

        return pNumber;
    }

    int genDuration() {
        //generating random durations between 20 and 7200 seconds
        int duration = (rand() % (7200 - 20 + 1) + 20);
        return duration;
    }

    //methods to find the averages for each customer type
    
    void findAveragesReg(){
        Call *tCall;
        RegularCustomer *tReg;
        int numCallsCust;
        int duration = 0;
        float bal = 0;
       
        //loops to get certain information for the output 
        for(int i =0; i < regVect.size()-1; i++){
            tReg = regVect[i];
            numCallsCust += tReg->getNumberOfCalls();
            tReg->computeBalance();
            bal += tReg->getBalance();
            
            for(int j = 0; j<tReg->getNumberOfCalls(); j++){
                tCall = tReg->getCall(j);
                duration += tCall->getDuration();              
            }
        
        }
        
        //affecting the field variable
        averageBalance = bal/regVect.size();
        //output
        cout << "Regular Number of Customers: " << endl;        
        cout<<"Average number of calls: "<<numCallsCust/regVect.size()<<endl;
        cout<<"Average duration of the call: "<<duration/numCallsCust<<" seconds"<<endl;
        cout<<"Average balance $" <<setprecision(4) <<averageBalance<<endl;
        cout << endl;
        
    }
           
    //methods to find the averages for each customer type
    
    void findAveragesPre(){
        int numCallsPreCust;
        int durationPre = 0;
        float bal = 0;
        Call *tPreCall;
        PremiumCustomer *tPre;
        
        //loops to get certain information for the output 
        for(int i =0; i < preVect.size()-1; i++){
            tPre = preVect[i];
            numCallsPreCust += tPre->getNumberOfCalls();
            tPre->computeBalance();
            bal += tPre->getBalance();
            
            for(int j = 0; j<tPre->getNumberOfCalls(); j++){
                tPreCall = tPre->getCall(j);
                durationPre += tPreCall->getDuration();              
            }
        
        }
        //affecting the field variable
        averagePreBalance = bal/preVect.size();
        //output
        cout << "Premium Number of Customers: " << endl;        
        cout<<"Average number of calls: "<<numCallsPreCust/preVect.size()<<endl;
        cout<<"Average duration of the call: "<<durationPre/numCallsPreCust<<" seconds"<<endl;
        cout<<"Average balance $"<<setprecision(4) <<averagePreBalance<<endl;
       
    }
    
    void printResult(){
        //output
        cout<<"Running simulation ..."<<endl;
        cout<<"Number of Regular Customers generated: "<<regVect.size()<<endl;
        cout<<"Number of Premium Customers generated: "<<preVect.size()<<endl;
        cout << endl;
        
        //methods calls for averages 
        findAveragesReg();
        //for premium
        findAveragesPre();
        
        cout<<endl;
        
        //using the balance field variable, comparing which customer saves more 
        if(averagePreBalance > averageBalance){
            cout<<"Regular customers on average save $"<<setprecision(4)<<averagePreBalance - averageBalance<<" compared to premium customers"<<endl;
        }
        else{
            cout<<"Premium customers on average save $"<<setprecision(4)<<averageBalance - averagePreBalance<<" compared to regular customers"<<endl;
        }
    }
};

int main(int argc, char** argv) {
    //Simulation object created
    Simulation *sim = new Simulation();
    sim->printResult();
    delete sim;
    return 0;
}