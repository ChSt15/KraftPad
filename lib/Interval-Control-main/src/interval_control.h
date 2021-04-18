#ifndef INTERVAL_CONTROL_H
#define INTERVAL_CONTROL_H



#include "Arduino.h"



class IntervalControl {
public:

    IntervalControl() {
        _lastRun_us = micros();
    }

    IntervalControl(float rate) {
        _lastRun_us = micros();
        if (rate != 0) setRate(rate);
    }

    /**
     * Sets the rate in Hz
     *
     * @param values rate in Hz
     * @return none.
     */
    inline void setRate(float rate_Hz) {_interval_us = (float)1000000L/rate_Hz;}

    /**
     * Gets the rate in Hz
     *
     * @param values none.
     * @return uint32_t.
     */
    inline uint32_t getRate() {return (float)1000000/_interval_us;}

    /**
     * Sets the interval in milliseconds
     *
     * @param values interval in milliseconds
     * @return none.
     */
    inline void setIntervalMillis(uint32_t interval_ms) {_interval_us = interval_ms*1000L; _lastRun_us = micros() - _interval_us;}

    /**
     * Sets the interval in microseconds
     *
     * @param values interval in microseconds
     * @return none.
     */
    inline void setIntervalMicros(uint32_t interval_us) {_interval_us = interval_us; _lastRun_us = micros() - _interval_us;}

    /**
     * Gets the interval in milliseconds
     *
     * @param values none.
     * @return interval in milliseconds.
     */
    inline uint32_t getIntervalMillis() {return _interval_us/1000;}

    /**
     * Gets the interval in microseconds
     *
     * @param values none.
     * @return interval in microseconds.
     */
    inline uint32_t getIntervalMicros() {return _interval_us;}

    /**
     * Returns the amount of time till the next run.
     * Negative number means the how long ago it should have ran
     *
     * @param values none
     * @return remaining time till next run.
     */
    inline int32_t getTimeRemainMicros() {return (int32_t) -_interval_us + micros() + _lastRun_us;}

    /**
     * Sets the way the system limits the runs.
     * If limit is true then this will only limit the rate
     * and if the run takes longer than the set interval the 
     * system will not compensate for the lost time.
     * If limit is false then if a run is late the system will make
     * the next runs come sooner. This causes abnormal intervals but
     * is usefull if somthing has to run a certain amount of times 
     * per second.
     *
     * @param values limit
     * @return none.
     */
    inline void setLimit(bool limit) {_limit = limit;}

    /**
     * This syncs the internal variables. This is usefull
     * if the limiter is off and to keep for huge amount of 
     * lost time to keep the system from running at max speed.
     * e.g. if say rate is set to 1khz and does not run for 1s
     * and syncRate() is not called then system will make up 
     * for the 1000 lost runs by running 1000 times as fast
     * as possible.
     *
     * @param values none
     * @return none.
     */
    inline void syncInternal() {_lastRun_us = micros();}

    /**
     * Waits till next Run.
     * See overload to use this time for other functions
     *
     * @param values none
     * @return none.
     */
    inline void waitTillNextRun() {

        while(micros() - _lastRun_us < _interval_us || _block);
        
        if (_limit) _lastRun_us = micros();
        else _lastRun_us += _interval_us;

    }

    /**
     * Waits till next Run but calls the given
     * method in the time it waits
     *
     * @param values method to run while waiting
     * @return none.
     */
    inline void waitTillNextRun(void (*callMethod)(void)) {

        while (micros() - _lastRun_us < _interval_us || _block) callMethod();
        
        if (_limit) _lastRun_us = micros();
        else _lastRun_us += _interval_us;

    }

    /**
     * Checks if it is time to run and returns
     * true if its time to run. 
     * If updateClock is true then the internal interval
     * is set for next run. If you only want to check if its
     * time to run then set updateClock to false;
     * Usefull for if statements. 
     * e.g:
     * 
     * if (x.isTimeToRun()) {
     *      Stuff to do periodically...
     * }
     *
     * @param values updateClock
     * @return none.
     */
    inline bool isTimeToRun(bool updateClock = true) {

        if (_block) return false;
        
        if (micros() - _lastRun_us >= _interval_us) {
            if (!_limit && updateClock) _lastRun_us +=_interval_us;
            else if (updateClock) _lastRun_us = micros();
            return true;
        } else return false;

    }

    /**
     * Checks if it is time to run and returns
     * true if its time to run. 
     * If updateClock is true then the internal interval
     * is set for next run. If you only want to check if its
     * run to run then set updateClock to false;
     * Usefull for if statements. 
     * This overload will write the time delta from the 
     * last run into the timeDelta variable. This way
     * the time from the last run is passed on.
     * e.g:
     * 
     * if (x.isTimeToRun()) {
     *      Stuff to do periodically...
     * }
     *
     * @param values updateClock
     * @return none.
     */
    inline bool isTimeToRun(uint32_t &timeDelta, bool updateClock = true) {

        if (_block) return false;
        
        timeDelta = micros() - _lastRun_us;
        if (timeDelta >= _interval_us) {
            if (!_limit && updateClock) _lastRun_us +=_interval_us;
            else if (updateClock) _lastRun_us = micros();
            return true;
        } else return false;

    }

    /**
     * If block is enabled then .isTimeToRun() will
     * always return false and .waitTillNextRun() will
     * never exit. This can be undone by calling the 
     * method and giving false as a parameter.
     *
     * @param values block
     * @return none.
     */
    inline void block(bool block) {
        _block = block;
    }


private:

    uint32_t _lastRun_us = 0;

    uint32_t _interval_us = 0;

    bool _limit = true;

    bool _block = false;

};





#endif