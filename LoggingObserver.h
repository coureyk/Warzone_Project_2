#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
class Subject; //Forward Declaration
//Observer abstract class
class Observer{
    public:
    ~Observer();
    virtual void Update(Subject* subject) = 0; //pure virtual function for update gamelog.txt
    protected:
    Observer();
};
//Subject class
class Subject{
    public:
    virtual void Attach(Observer* o); //Function to attach Observer
    virtual void Detach(Observer* o); //Function to detach Observer
    virtual void Notify(); //Function to notify update
    Subject();
    ~Subject();
    private:
    std::vector<Observer*> *_observers; //List of observer
};
//ILoggable abstract class to convert string
class ILoggable{
    protected:
    ILoggable();
    public:
    ~ILoggable();
    virtual std::string stringToLog()=0; //pure virtual function to convert string
};
//LogObserver class to implement the update text to logfile
class LogObserver : public Observer{
    private:
    std::ofstream logFile; //txt file name
    Subject* _subject; //subject
public:
    LogObserver(Subject* s);
    void Update(Subject* subject); //override Update function
    ~LogObserver();
};
#endif