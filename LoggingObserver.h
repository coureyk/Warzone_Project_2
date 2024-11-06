#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
class Subject;
class Observer{
    public:
    ~Observer();
    virtual void Update(Subject* subject) = 0;
    protected:
    Observer();
};
class Subject{
    public:
    virtual void Attach(Observer* o);
    virtual void Detach(Observer* o);
    virtual void Notify();
    Subject();
    ~Subject();
    private:
    std::vector<Observer*> *_observers;
};
class ILoggable{
    protected:
    ILoggable();
    public:
    ~ILoggable();
    virtual std::string stringToLog()=0;
};
class LogObserver : public Observer{
    private:
    std::ofstream logFile;
    Subject* _subject;
public:
    LogObserver(Subject* s);
    void Update(Subject* subject);
    ~LogObserver();
};
#endif