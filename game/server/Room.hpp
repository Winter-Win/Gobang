#pragma once

#include <iostream>
#include <pthread.h>
#include <string>

#define BLACK 'X'
#define WHITE 'O'
#define SIZE 5

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

public:
    Room()
    {
    }

    Room(uint32_t &id1, uint32_t &id2):one(id1),two(id2)
    {
        piece[0] = 'X';
        piece[1] = 'O';
        memset(board, ' ', sizeof(board));
        result = 'N';
        current = one; //默认等于one
        pthread_mutex_init(&lock, NULL);
    }

   // void Lock()
   // {
   //     pthread_mutex_lock(&lock);
   // }

   // void Unlock()
   // {
   //     pthread_mutex_unlock(&lock);
   // }

    void Board(string &_board)
    {
        for(auto i = 0; i < SIZE; i++)
        {
            for(auto j = 0; j < SIZE; j++)
            {
                _board.push_back(board[i][j]);
            }
        }
    }

    char Piece(uint32_t &id)
    {
        int pos = (id == one ? 0 : 1);
        return piece[pos];
    }

    bool IsMyTurn(uint32_t &id)
    {
        return id == current ? true : false;
    }

    void Step(uint32_t &id, const int &x, const int &y)
    {
        if(current == id)
        {
            int pos = (id == one ? 0 : 1); //piece[pos]
            board[x][y] = piece[pos];
            cout << board[x][y] << "键入的坐标是 : " << x + 1 << " " << y + 1 << endl;
            current = (id == one ? two : one);
            //result = Judge();

            result = Judge(x, y);
        }
    }

    char GameResult(uint32_t &id)
    {
        return result;
    }




bool is_win(const int& x,const int& y)
{
    int count = 0;
    int flag = 1;
    char cur = board[x][y];

    int i = 0;
    int j = 0;
    //垂直上
    for(i = x-1, j = y; i >= 0 && count++ < 5; i--)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    //垂直下
    count = 0;
    for(i = x + 1, j = y; i < SIZE && count ++ < 5; i++ )
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    count = 0;
    if( flag >= 5 )
        return true;
    else
        flag = 1;

    //水平右
    for(i = x, j = y + 1; j < SIZE && count ++ < 5 ; j++ )
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    //水平左
    count = 0;
    for(i = x, j = y - 1; j >= 0 && count++ < 5 ; j--)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    count = 0;
    if(flag >= 5)
        return true;
    else
        flag = 1;

    //主右下
    for(i = x + 1, j = y + 1; i < SIZE && j < SIZE && count ++ < 5 ; i++ , j++)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    //主左上
    count = 0;
    for(i = x - 1, j = y - 1; i >= 0 && j >= 0 && count++ < 5 ; i-- ,j--)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    count = 0;
    if(flag >= 5)
        return true;
    else
        flag = 1;

    //斜右上
    for(i = x -1, j = y + 1; i >= 0 && j < SIZE && count++ < 5; i--,j++)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    //斜左下
    count = 0;
    for(i = x + 1, j = y - 1; i < SIZE && y >= 0 && count++ < 5; i++,j--)
    {
        if(board[i][j] == cur)
            flag++;
        else
            break;
    }

    count = 0;
    if(flag >= 5)
        return true;
    else
        flag = 1;


    return false;
}




    char Judge(const int& x, const int& y)
    {
        if(is_win(x, y))
            return board[x][y];

        for(auto i = 0; i < SIZE; ++i)
        {
            for(auto j = 0; j < SIZE; ++j)
            {
                if(board[i][j] == ' ')
                {
                    return 'N';
                }
            }
        }

        return 'E';
    }

//    char Judge()
//    {
//        int row = SIZE;
//        int col = SIZE;
//        //判断每一行
//        for(auto i = 0; i < row; ++i)
//        {
//            if(board[i][0] != ' ' && \
//               board[i][0] == board[i][1] && \
//               board[i][1] == board[i][2] && \
//               board[i][2] == board[i][3] && \
//               board[i][2] == board[i][4])
//            {
//                return board[i][0];
//            }
//        }
//        //判断每一列
//        for(auto i = 0; i < col; ++i)
//        {
//            if(board[0][i] != ' ' && \
//               board[0][i] == board[1][i] && \
//               board[1][i] == board[2][i] && \
//               board[2][i] == board[3][i] && \
//               board[3][i] == board[4][i])
//            {
//                return board[0][i];
//            }
//        }
//        //判断对角线
//        if(board[0][0] != ' ' && \
//           board[0][0] == board[1][1] && \
//           board[1][1] == board[2][2] && \
//           board[2][2] == board[3][3] && \
//           board[3][3] == board[4][4])
//        {
//            return board[0][0];
//        }
//        if(board[0][4] != ' ' && \
//           board[0][4] == board[1][3] && \
//           board[1][3] == board[2][2] && \
//           board[2][2] == board[3][1] && \
//           board[3][1] == board[4][0])
//        {
//            return board[0][4];
//        }
//        for(auto i = 0; i < row; ++i)
//        {
//            for(auto j = 0; j < col; ++j)
//            {
//                if(board[i][j] == ' ')
//                {
//                    return 'N';
//                }
//            }
//        }
//        return 'E';
//    }

    ~Room()
    {
        pthread_mutex_destroy(&lock);
    }
};
