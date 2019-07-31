# Gobang
## 五子对战
- 其实在做这个之前呢，我做个一个三子棋的一个小项目，当时玩的还是可以，但是并没有实现太复杂，只是单纯的设计为在本地编译成可执行程序，电脑端的落子都是随机的。当时只是实现了可以人机对战的形式，判断输赢。
- 三子棋的完成是在VS 2013下面完成的，今天这个项目呢，就比较实在了，是在Linux下完成的，花费了比较多的时间来研究。下面就由我来一一介绍吧。
- 三子棋了解：**https://blog.csdn.net/qq_41562665/article/details/82962428**

**在做这个项目之前，需要做以下准备工作。**
-  **了解RPC**
1. RPC主要是为了解决下面两个问题：
解决分布式系统中，服务之间的调用问题。
远程调用时，能够像本地调用一样方便，让调用者感知不到远程调用的逻辑。


2. RPC调用和普通本地调用有什么区别


本地调用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190705175548102.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNTYyNjY1,size_16,color_FFFFFF,t_70)
   
 远程过程调用：
   ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190705175604811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNTYyNjY1,size_16,color_FFFFFF,t_70)


- **rpc和本地过程(函数)调用的区别：**
  ① 本地过程调用，就是我们平时所写的函数接口调用。
  ② 本地过程调用的方法实现在本地，而远程过程调用不光方法实现在远端，甚至执行过程都在远端主机，本地只拿一个结果。
- **为什么使用rpc框架？**
1、要实现网络通信，就要解决：
① 网络通信的问题（通过tcp/udp协议搞定）
② 协议的定制 
③ 为了考虑数据格式的问题，就得考虑序列化和反序列化
2、为了把重点高度集中在五子棋的业务逻辑上，所以才使用了rest_rpc.rest_rpc是一个轻量化的rpc框架，市面上还有一些其他的rpc框架，如百度开源的rpc框架sofa-bprpc。
rest_rpc是写得比较轻的，只要下载下来就能使用，但是也有一些基本的要求，它的语法用了c++11和c++17的特性，所以我们要编译成功，就需要使用高版本的编译器(gcc7.1以上)。
3、rpc底层用了boost库的网络代码，boost库的serverI/O解决网络问题，rpc只专注于远程过程调用，我们只写游戏逻辑，这样实现这个游戏起来就相对比较简单了。

- **工具安装**
1. 安装cmake
安装教程：**https://blog.csdn.net/fancyler/article/details/78009812**
2. 安装高版本gcc
安装教程：**https://www.jianshu.com/p/76fb9b6a781b**
3. 安装boost库
安装教程：**https://www.cnblogs.com/xiehongfeng100/p/4375613.html?tdsourcetag=s_pctim_aiomsg**

#### 设计游戏：
**整个游戏是需要有注册，登录，匹配，玩游戏**(棋盘获取，棋子获取，落子信息，判断输赢)**，退出几个功能的，当然有了这样一些机制，很多功能都是可以继续增加的。**
- **服务端的管理各个信息的类设计**
1. 游戏大厅信息管理
**Hall.hpp**
```
class Hall
{
private:
    PlayerManager pm; //玩家信息
    RoomManager rm; //房间信息
    vector<vector<uint32_t> > match_pool; //匹配池
    int match_num;
    pthread_mutex_t match_lock;
    pthread_cond_t match_cond;
};
```
2. 游戏玩家信息
**Player.hpp**
```
typedef enum
{
    OFFLINE = 0,
    ONLINE,
    MATCHING,
    PLAYING,
}status_t;


class Player
{
private: //相当于数据库，以及表
    string name;
    string passwd;
    uint32_t id; //

    int win;
    int lose;
    int tie; //平局

    status_t st;
    uint32_t room_id;
};
```
3. 游戏房间信息
**Room.hpp**
```
#define BLACK 'X'
#define WHITE 'O'
#define SIZE 9

class Room
{
private:
    uint32_t one;
    uint32_t two;
    char piece[2];
    uint32_t current;
    char board[SIZE][SIZE];
    char result; // X, O, E, N

    pthread_mutex_t lock;
};
```
4. 游戏玩家管理
**PlayerManage.hpp**
```
class PlayerManager
{
private:
    unordered_map<uint32_t, Player> player_set;
    uint32_t assign_id;

    pthread_mutex_t lock;
};
```
5. 游戏房间管理
**RoomManage.hpp**
```
class RoomManager
{
private:
    unordered_map<uint32_t, Room> room_set;
    uint32_t  assign_id;

    pthread_mutex_t lock;
};
```


