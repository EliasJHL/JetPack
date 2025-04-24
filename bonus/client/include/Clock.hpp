#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <SFML/System.hpp>

class Clock {
public:
    Clock(float interval = 0.3f) : mInterval(interval) {}

    void restart() {
        mClock.restart();
    }

    bool isElapsed() const {
        return mClock.getElapsedTime().asSeconds() >= mInterval;
    }

    void setInterval(float interval) {
        mInterval = interval;
    }

    float getInterval() const {
        return mInterval;
    }

private:
    sf::Clock mClock;
    float mInterval;
};

#endif /* CLOCK_HPP_ */