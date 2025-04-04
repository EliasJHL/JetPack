#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <SFML/System.hpp>

class Clock {
public:
    Clock(float interval = 0.3f) : mInterval(interval) {}

    // Redémarre l'horloge
    void restart() {
        mClock.restart();
    }

    // Vérifie si l'intervalle est écoulé
    bool isElapsed() const {
        return mClock.getElapsedTime().asSeconds() >= mInterval;
    }

    // Définit un nouvel intervalle
    void setInterval(float interval) {
        mInterval = interval;
    }

    // Retourne l'intervalle actuel
    float getInterval() const {
        return mInterval;
    }

private:
    sf::Clock mClock;
    float mInterval; // Intervalle en secondes
};

#endif /* CLOCK_HPP_ */