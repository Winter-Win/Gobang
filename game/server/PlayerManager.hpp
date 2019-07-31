#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>   //  std::pair

#include "Log.hpp"
#include "Player.hpp"


#define DEFAULT_ID 10000

using namespace std;

class PlayerManager
{
private:
    unordered_map<uint32_t, Player> player_set;
    uint32_t assign_id;

    pthread_mutex_t lock;
public:
    PlayerManager():assign_id(DEFAULT_ID)
    {
    }

    void Lock()
    {
        pthread_mutex_lock(&lock);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&lock);
    }

    //插入用户以及注册出来了
    uint32_t InsertPlayer(string &name, string &passwd)
    {
        Lock();
        uint32_t id_ = assign_id++;
        Player p(name, passwd, id_);
        //player_set.insert(make_pair<uint32_t, Player>(id_, p));
        player_set.insert({id_, p});
        Unlock();
        LOG(INFO, "玩家正在注册插入中...");
        return id_;
    }

    //查找，负责在登录时用
    uint32_t SearchPlayer(uint32_t &id, string& passwd)
    {
        //1、迭代器
        //2、find
        Lock();
        auto iter = player_set.find(id);
        if(iter == player_set.end())
        {
            LOG(WARNING, "查找用户失败...");
            Unlock();
            return 1;
        }

        Player &p = iter->second; //拿到用户的第二个信息
        if(p.Passwd() != passwd)
        {
            Unlock();
            LOG(WARNING, "密码错误...");
            return 2;
        }
        p.Online(); //置为在线
        Unlock();
        LOG(INFO, "用户登录成功...");
        return id;
    }
    
//    void SignalPlayer(uint32_t &one, uint32_t &two)
//    {
//        player_set[one].Signal();
//        player_set[two].Signal();
//    }

    void SetPlayStatus(uint32_t &one, uint32_t &two)
    {
        Lock();
        player_set[one].Playing();
        player_set[two].Playing();
        Unlock();
    }

    void SetPlayRoom(uint32_t &room_id, uint32_t &one, uint32_t &two)
    {
        Lock();
        player_set[one].SetRoom(room_id);
        player_set[two].SetRoom(room_id);
        Unlock();
    }

    void SetMatching(uint32_t &id)
    {
        Lock();
        player_set[id].Matching();
        Unlock();
    }

    void SetOnline(uint32_t &id)
    {
        Lock();
        player_set[id].Online();
        Unlock();
    }

//    bool PlayerWait(uint32_t &id)
//    {
//        if(ETIMEDOUT == player_set[id].Wait())
//        {
//            cout << "Play time out..." << endl;
//            return false;
//        }
//        cout << "Play wakeup..." << endl;
//        return true;
//    }
//

    uint32_t GetPlayerRoomId(uint32_t &id)
    {
        Lock();
        uint32_t _room_id = player_set[id].Room();
        Unlock();
        return _room_id;
    }

    int GetRate(uint32_t &id)
    {
        Lock();
        int rate = player_set[id].Rate();
        Unlock();
        return rate;
    }

    int Ready(uint32_t &id)
    {
        Lock();
        int st = player_set[id].Status();
        Unlock();
        return st;
    }

    ~PlayerManager()
    {
        pthread_mutex_destroy(&lock);
    }
};
