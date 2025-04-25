/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Menu
*/

#include "GameManager.hpp"

void GameManager::HandleClick(void)
{
    if (mEvent.type == sf::Event::MouseButtonPressed && mEvent.mouseButton.button == sf::Mouse::Left && mGameMode == LOBBY) {
        if (mLobbyMenu == BASE) {
            for (int i = 0; i < mStartButtons.size(); i++) {
                if (mStartButtons[i]->isClicked(mEvent.mouseButton.x, mEvent.mouseButton.y, mFont)) {
                    if (i == 0)
                        mLobbyMenu = LobbyMenu::CREATE_SALON;
                    if (i == 1) {
                        std::string m = "RER\r\n";
                        send(mPlayerSocket, m.c_str(), m.length(), 0);
                        mLobbyMenu = LobbyMenu::LIST_SALON;
                    }
                    if (i == 2)
                        mWindow.close();
                }
            }
        }
        if (mLobbyMenu == CREATE_SALON) {
            for (int i = 0; i < mCreateButtons.size(); i++) {
                if (mCreateButtons[i]->isClicked(mEvent.mouseButton.x, mEvent.mouseButton.y, mFont)) {
                    if (i == 0 && mCreateServerPort > 1)
                        mCreateServerPort -= 1;
                    if (i == 1 && mCreateServerPort < 20)
                        mCreateServerPort += 1;
                    if (i == 2) {
                        mCreateServerName = mServerNameDisplay.getString();
                        if (mCreateServerName.empty() || mCreateServerName.length() < 2) {
                            std::cout << "Invalid Salon Name" << std::endl;
                            continue;
                        }
                        std::string message = "CRM " + mCreateServerName + " " + std::to_string(mCreateServerPort) + "\r\n";
                        send(mPlayerSocket, message.c_str(), message.length(), 0);
                        std::cout << "Server sended to be created..." << std::endl;
                        std::cout << "Trying to join the room..." << std::endl;
                        message = "JOS " + mCreateServerName + "\r\n";
                        send(mPlayerSocket, message.c_str(), message.length(), 0);
                        mAlreadyJoining = true;
                    }
                }
            }
        }
        if (mLobbyMenu == LIST_SALON) {
            for (int i = 0; i < mJoinButtons.size(); i++) {
                if (mJoinButtons[i]->isClicked(mEvent.mouseButton.x, mEvent.mouseButton.y, mFont)) {
                    if (i == 0) {
                        mActiveRooms.clear();
                        std::string m = "RER\r\n";
                        send(mPlayerSocket, m.c_str(), m.length(), 0);
                    }
                }
            }
        }
    }
}

void GameManager::handle_menu_events(void)
{
    while (mWindow.pollEvent(mEvent)) {
        if (mEvent.type == sf::Event::Closed) {
            std::string m = "DEC\r\n";
            send(mPlayerSocket, m.c_str(), m.length(), 0);
            mRunning = false;
            mWindow.close();
        }
        if (mEvent.type == sf::Event::TextEntered && mHasUsername && !mGameReady && mLobbyMenu == CREATE_SALON) 
        {
            if (mEvent.text.unicode == '\b' && !mInputServerName.isEmpty()) {
                mInputServerName.erase(mInputServerName.getSize() - 1, 1);
            } else if (mEvent.text.unicode != '\b' && mEvent.text.unicode != '\n' && mEvent.text.unicode != '\r' && mInputServerName.getSize() < 20) {
                mInputServerName += mEvent.text.unicode;
            }
            mServerNameDisplay.setString(mInputServerName);
        }
        if (mEvent.type == sf::Event::MouseButtonPressed) {
            HandleClick();
        }
    }
}

void GameManager::DrawRect(Rect rect)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(rect.getWidth(), rect.getHeight()));
    rectangle.setPosition(rect.getX(), rect.getY());
    rectangle.setFillColor(sf::Color(rect.getColor().getR(), rect.getColor().getG(), rect.getColor().getB(), rect.getColor().getA()));
    mWindow.draw(rectangle);
}

void GameManager::DrawText(Text text)
{
    sf::Text sfText;
    sfText.setFont(mFont);
    sfText.setString(text.getText());
    sfText.setCharacterSize(text.getSize());
    sfText.setFillColor(sf::Color(text.getColor().getR(), text.getColor().getG(), text.getColor().getB(), text.getColor().getA()));
    if (text.isCenteredX()) {
        sfText.setPosition(text.getX() - sfText.getGlobalBounds().width / 2, text.getY());
    } else if (text.isCenteredY()) {
        sfText.setPosition(text.getX(), text.getY() - sfText.getGlobalBounds().height / 2);
    } else {
        sfText.setPosition(text.getX(), text.getY());
    }

    mWindow.draw(sfText);
}

void GameManager::DrawButon(Bouton *btn)
{
    sf::Text sfText;
    sfText.setFont(mFont);
    sfText.setString(btn->getText());
    sfText.setCharacterSize(btn->getSize());
    sfText.setFillColor(sf::Color(btn->getColor().getR(), btn->getColor().getG(), btn->getColor().getB(), btn->getColor().getA()));
    if (btn->isCenteredX()) {
        sfText.setPosition(btn->getX() - sfText.getGlobalBounds().width / 2, btn->getY());
    } else if (btn->isCenteredY()) {
        sfText.setPosition(btn->getX(), btn->getY() - sfText.getGlobalBounds().height / 2);
    } else {
        sfText.setPosition(btn->getX(), btn->getY());
    }

    mWindow.draw(sfText);
}

void GameManager::DisplayNewSalon(Rooms room)
{
    static int yOffset = 100;
    static int count = 0;
    
    int y = 100 + (count * 120);
    
    DrawRect(Rect(100, y, 1000, 100, Color(50, 50, 70, 200)));
    
    DrawText(Text(150, y + 20, 30, "Room " + room.name, false, false, Color(255, 255, 255, 255)));
    
    std::string playerInfo = "Players connected " + std::to_string(room.active_nb_players) + 
                             " I " + std::to_string(room.max_nb_players);
    DrawText(Text(150, y + 60, 25, playerInfo, false, false, Color(200, 200, 255, 255)));
    
    if (room.active_nb_players < room.max_nb_players) {
        Bouton joinButton(900, y + 50, 25, "Join", false, true, Color(100, 255, 100, 255), room.name);
        DrawButon(&joinButton);
        if (joinButton.isClicked(mEvent.mouseButton.x, mEvent.mouseButton.y, mFont)) {
            if (!mAlreadyJoining) {
            std::string message = "JOS " + room.name + "\r\n";
            send(mPlayerSocket, message.c_str(), message.length(), 0);
            mAlreadyJoining = true;
            std::cout << message << std::endl;
            } else {
                std::cout << "ALREADY JOINING" << std::endl;
            }
        }
    } else {
        DrawText(Text(900, y + 50, 25, "Full", false, true, Color(255, 100, 100, 255)));
    }
    
    count++;
    
    if (count >= mActiveRooms.size()) {
        count = 0;
    }
}

void GameManager::MenuDraw(void)
{
    if (mLobbyMenu == BASE) {
        DrawText(Text(600, 70, 40, "JetPack Multiplayer", true, true, Color(255, 255, 255, 255)));
        for (const auto& b : mStartButtons) {
            DrawButon(b.get());
        }
    } else if (mLobbyMenu == CREATE_SALON) {
        mServerNameDisplay.setPosition({100, 230});
        mServerNameDisplay.setCharacterSize(20);
        mServerNameDisplay.setColor({255,255,255,255});
        mServerNameDisplay.setString(mInputServerName);
        DrawText(Text(100, 60, 40, "Create Room", false, true, Color(255, 255, 255, 255)));
        DrawText(Text(95, 200, 30, "Enter Server Name", false, true, Color(255, 255, 255, 255)));
        DrawText(Text(140, 330, 30, std::to_string(mCreateServerPort), true, true, Color(255, 255, 255, 255)));
        DrawText(Text(95, 300, 30, "Enter nb players", false, true, Color(255, 255, 255, 255)));
        for (const auto& b : mCreateButtons) {
            DrawButon(b.get());
        }
        mWindow.draw(mServerNameDisplay);
    } else {
        DrawText(Text(600, 40, 30, "Join a room", true, true, Color(255, 255, 255, 255)));
        for (auto const& x : mActiveRooms) {
            DisplayNewSalon(x.second);
        }
        for (const auto& b : mJoinButtons) {
            DrawButon(b.get());
        }
    }
}