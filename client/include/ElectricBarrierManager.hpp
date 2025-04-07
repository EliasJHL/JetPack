/*
** ElectricBarriersManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:30:56 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:12 2025 Elias Josué HAJJAR LLAUQUEN
*/

// basé sur le sigleton

#ifndef COINSMANAGER_HPP_
# define COINSMANAGER_HPP_

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "ElectricBarrier.hpp"

class ElectricBarriersManager {
    public:
        static ElectricBarriersManager *getInstance(void);
        void createElectricBarrier(const std::string &name, int id);
        void removeElectricBarrier(int id);
        ElectricBarrier *getElectricBarrier(int id);
        std::vector<ElectricBarrier *> getAllElectricBarriers(void);
    private:
        ElectricBarriersManager() {};
        ~ElectricBarriersManager() = default;

        ElectricBarriersManager(ElectricBarriersManager &other) = delete;
        void operator=(const ElectricBarriersManager &) = delete;
        std::unordered_map<int, std::unique_ptr<ElectricBarrier>> mElectricBarriers;
        static ElectricBarriersManager *mElectricBarriersManager;
        static std::mutex mMutex;
};

#endif /* !COINSMANAGER_HPP_ */
