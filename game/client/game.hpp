#pragma once

#include <iostream>
#include <stdlib.h>
#include <rpc_client.hpp>
#include <chrono>
#include <fstream>
#include "codec.h"
using namespace std;
using namespace std::chrono_literals;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;


uint32_t Register(const string &ip, const int &port, string &name, string &passwd, uint32_t &id)
{
    cout << "请输入的昵称: ";
    cin >> name;
    cout << "请输入密码: ";
    cin >> passwd;
    string passwd_;
    cout << "请再次输入密码: ";
    cin >> passwd_;
    if(passwd != passwd_)
    {
        cout << "密码不一致！" << endl;
        return 1;
    }

	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 2;
    	}
        id = client.call<uint32_t>("RpcRegister", name, passwd);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
	}
    //return id;
    return 0;
}

uint32_t Login(const string &ip, int &port)
{
    uint32_t id;
    string passwd;

    cout << "请输入你的登录ID: ";
    cin >> id;
    cout << "请输入你的登录密码: ";
    cin >> passwd;

    uint32_t result = 0;
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 3;
    	}
        result = client.call<uint32_t>("RpcLogin", id, passwd);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }

    return result;
}

bool PushMatchPool(uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 4;
    	}
        return client.call<bool>("RpcMatchAndWait", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
    return false;
}

bool PopMatchPool(uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 5;
    	}
        return client.call<bool>("RpcPopMatchPool", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
    return false;
}

int CheckReady(uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 6;
    	}
        return client.call<int>("RpcPlayerReady", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }

    return 1;
}

bool Match(uint32_t &id, const string &ip, int port)
{
    PushMatchPool(id, ip, port);
    int count = 20;
    while(1)
    {
        int result = CheckReady(id, ip, port);
        //cout << "check: " << result << endl;
        if(result == 3)
        {
            cout << "匹配成功！" << endl;
            return true;
        }
        else if(result == 1)
        {
            cout << "匹配失败！" << endl;

            //PopMatchPool(id, ip, port);
            return false;
        }
        else
        {
            printf("匹配中 ........ %2d\r", count--);
            fflush(stdout);
            if(count < 0)
            {
                cout << endl;
                cout << "匹配超时！" << endl;

                PopMatchPool(id, ip, port);  //此处是清空匹配池，不然就会下一个来的时候就直接匹配上了，但是实际上是没有人和他匹配的

                //remove client
                
                break;
            }
            sleep(1);
        }
    }
    return false;
}

int GetBoard(uint32_t &room_id, const string &ip, int &port, string &board)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 7;
    	}
        board =  client.call<string>("RpcBoard", room_id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

uint32_t GetMyRoomId(uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 8;
    	}
        return client.call<uint32_t>("RpcPlayerRoomId", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }

}

char GetMyPiece(uint32_t &room_id, uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 9;
    	}
        return client.call<char>("RpcPlayerPiece",room_id, id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

void ShowBoard(string &board) // 针对于棋盘大于10的
{
//    cout << "    ";
//    for(auto i = 1; i <= 5; ++i)
//    {
//        cout << i << "   ";
//    }
//    cout << endl;
//    for(auto i = 0; i <= 5; ++i)
//    {
//        cout << "----";
//    }
//    cout << endl;
//
//    int size = board.size();
//    int basic_size = 5;
//    for(auto i = 0; i < size/basic_size; ++i)
//    {
//        cout << i+1 << " |";
//        for(auto j = 0; j < basic_size; ++j)
//        {
//            cout << " " << board[i*basic_size + j] << " |";
//        }
//        cout << endl;
//        for(auto i = 0; i <= 5; ++i)
//        {
//            cout << "----";
//        }
//        cout << endl;
//    }
    

    int size = board.size();
    int basic_size = sqrt(size);
    cout << "    ";
    for(auto i = 1; i <= basic_size; ++i)
    {
        cout << i << "   ";
    }
    for(auto i = 10; i <= basic_size; ++i)
    {
        cout << i << "  ";
    }
    cout << endl;
    for(auto i = 0; i <= basic_size; ++i)
    {
        cout << "----";
    }
    cout << endl;

    for(auto i = 0; i < basic_size; ++i)
    //for(auto i = 0; i < 9; ++i)
    {
        cout << i+1 << " |";
        for(auto j = 0; j < basic_size; ++j)
        {
            cout << " " << board[i*basic_size + j] << " |";
        }
        cout << endl;
        for(auto i = 0; i <= basic_size; ++i)
        {
            cout << "----";
        }
        cout << endl;
    }

    //for(auto i = 9; i < basic_size; ++i)
    //{
    //    cout << i+1 << "|";
    //    for(auto j = 0; j < basic_size; ++j)
    //    {
    //        cout << " " << board[i*basic_size + j] << " |";
    //    }
    //    cout << endl;
    //    for(auto i = 0; i <= basic_size; ++i)
    //    {
    //        cout << "----";
    //    }
    //    cout << endl;
    //}
}

bool IsMyTurn(uint32_t &room_id, uint32_t &id, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 10;
    	}
        return client.call<bool>("RpcIsMyTurn",room_id, id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
    return false;
}

bool PosIsRight(string &board, int x, int y)
{
    int size = board.size();
    int basic_size = sqrt(size);
    int pos = (x-1)*basic_size + (y-1);
    return board[pos] == ' ' ? true : false;
}

int Step(uint32_t &room_id, uint32_t &id, int x, int y, const string &ip, int &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 11;
    	}
        client.call<void>("RpcStep",room_id, id, x-1, y-1);
        return 0;
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

char Judge(uint32_t &room_id, uint32_t &id, const string &ip, int port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 12;
    	}
        return client.call<char>("RpcJudge",room_id, id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

void PlayGame(uint32_t &id, const string &ip, int &port)
{
    //cout << "匹配成功，请开始游戏..." << endl;
    int x, y;
    char result = 'N';
    string board;
    uint32_t room_id = GetMyRoomId(id, ip, port);
    cout << "client room_id: " << room_id << endl;
    if(room_id < 1024)
    {
        return;
    }
    cout << "room_id: " << room_id << endl;
    char piece = GetMyPiece(room_id, id, ip, port);
    cout << "piece: " << piece << endl;
    while(1)
    {
        system("clear");

        GetBoard(room_id, ip, port, board);
        ShowBoard(board);
        if((result = Judge(room_id, id, ip, port)) != 'N')
        {
            break;
        }
        if(!IsMyTurn(room_id, id, ip, port) )
        {
            cout << "对方正在思考，请稍等..." << endl;
            sleep(1);
            continue;
        }
        cout << "请输入你的落子位置:>" ;
        cin >> x >> y;
        int size = board.size();
        int basic_size = sqrt(size);
        if(x >= 1 && x <= basic_size && y >= 1 && y <= basic_size)
        {
            if(!PosIsRight(board, x, y))
            {
                cout << "你输入的位置已经被占用，请重新输入！" << endl;
            }
            else
            {
                Step(room_id, id, x, y, ip, port);
                result = Judge(room_id, id, ip, port);
                if(result != 'N')
                {
                    break;
                }
            }
        }
        else
        {
            cout << "你输入的位置有误，请重新输入！" << endl;
        }
    }

    if(result == 'E')
        cout << "平局，都挺好！" << endl;
    else if(result == piece)
        cout << "你赢了，恭喜！" << endl;
    else
        cout << "你输了，再来一局吧！" << endl;
}

void Game(uint32_t &id, const string &ip, int &port)
{
    int select = 0;
    volatile bool quit = false;
    while(!quit)
    {
        cout << "#########################################" << endl;
        cout << "####    1. 匹配           2. 退出    ####" << endl;
        cout << "#########################################" << endl;
        cout << "请选择: ";
        cin >> select;
        switch(select)
        {
            case 1:
                {
                    if(Match(id, ip, port)) //匹配
                    {
                        PlayGame(id, ip, port);
                    }
                    else
                    {
                        cout << "匹配失败，请重试！" << endl;
                    }
                }
                break;
            case 2:
                quit = true;
                break;
            default:
                cout << "选择错误，请重新输入！" << endl;
                break;
        }
    }
}
