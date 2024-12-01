#include "LoggingObserver.h"

Observer::Observer(){
}; 
Observer::~Observer(){
};
Subject::Subject() : _observers(new std::vector<Observer*>) {}
Subject::~Subject(){ 
    for (Observer* observer : *_observers) {
        observer->Update(nullptr); // Notify observers about the destruction
    }
	delete _observers;
};
void Subject::Attach(Observer* o){
  _observers->push_back(o);
};
void Subject::Detach(Observer* o){
  _observers->erase(std::remove(_observers->begin(), _observers->end(),o), _observers->end());
};
void Subject::Notify(){
   /* for (auto it = _observers->begin(); it != _observers->end(); ++it) {
        
        (*it)->Update(this);  
        } */  

         try {
        for (auto it = _observers->begin(); it != _observers->end(); ++it) {
            (*it)->Update(this);
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during Notify: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception during Notify." << std::endl;
    }
};
ILoggable::ILoggable(){

};
ILoggable::~ILoggable(){

};

LogObserver::LogObserver(Subject *s) {
    logFile.open("gamelog.txt", std::ios::app);
    _subject = s;
    _subject->Attach(this);
};
void LogObserver::Update(Subject* subject){
     if (!subject) {
        return; // Ignore updates from a destroyed subject
    }
    ILoggable* loggable = dynamic_cast<ILoggable*>(subject);
    if (loggable && logFile.is_open()) {
        std::string logEntry = loggable->stringToLog();
      
        if (!logEntry.empty()) {  // Ensure the log entry is not empty
            logFile << logEntry << std::endl;
        }
    }
};
LogObserver::~LogObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
    _subject->Detach(this);
};
