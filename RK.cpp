#include <math.h>
#include "./vec.h"

/*n元連立微分方程式を解くのに使える*/
/*ルンゲクッタ法でvy(x+h)の値を求めるために使われる*/  /*dvy/dx = f(x, vy)*/  //ただしvyはxの関数を並べたベクトルで、vfもベクトル
template<int n>
Vec<n> dif_RKn(double x, Vec<n> vy, double h, Vec<n>(*vf)(double, Vec<n>))
{
    Vec<n> vk1 = vf(x      ,   vy              );
    Vec<n> vk2 = vf(x + h/2,   vy  +  h/2 * vk1);
    Vec<n> vk3 = vf(x + h/2,   vy  +  h/2 * vk2);
    Vec<n> vk4 = vf(x + h  ,   vy  +  h   * vk3);

    return h/6*(vk1 + 2*vk2 + 2*vk3 + vk4);
}


/*dif_RKnを何度も呼び出すことでn元連立微分方程式を解く*/
/*xの初期値   xの終了値   y(xの関数を並べたベクトル)の初期値   区分間隔   関数f(x, y)*/   // 戻り値 y(x_end)
template<int n>
Vec<n> RKn(double x_begin, double x_end, Vec<n> vy0, double h, Vec<n>(*vf)(double, Vec<n>))
{
    double num = (x_end - x_begin)/h;
    Vec<n> vy = vy0;

    for(int i=0; i<num; i++)
    {
        double x = x_begin + h*i;
        vy += dif_RKn(x, vy, h, vf);
    }
    return vy;
}

#include <fstream>
/*xとyの第一成分をcsvファイルに書き出す*/
template<int n>
void RKn_csv(std::string filename, double x_begin, double x_end, Vec<n> vy0, double h, Vec<n>(*vf)(double, Vec<n>))
{
    double num = (x_end - x_begin)/h;
    Vec<n> vy = vy0;

    std::ofstream file(filename + ".csv");

    for(int i=0; i<num; i++)
    {
        double x = x_begin + h*i;
        vy += dif_RKn(x, vy, h, vf);
        file << x_begin + h*(i+1) << ' ' << vy[0] << std::endl;
    }
    file.close();
}





/*RKnの引数の関数の例*/ //  dy/dx = x*x*x*x
Vec<1> vf11(double x, Vec<1> vy)
{
    /*Vecクラスだが、1次元ベクトルなので実質double型と同じ計算*/
    Vec<1> ret;
    ret[0] = x*x*x*x; //  dy/dx = x*x*x*x
    return ret;
}

/*RKnの引数の関数の例*/ //  dy/dx = y;
Vec<1> vf12(double x, Vec<1> vy)
{
    /*Vecクラスだが、1次元ベクトルなので実質double型と同じ計算*/
    Vec<1> ret;
    ret[0] = vy[0]; //  dy/dx = y
    return ret;
}

/*RKnの引数の関数の例*/ // 2階微分方程式 d2y/dx2 = y ---> 分解 --->  dy/dx = ydot,  d(ydot)/dx = y 
Vec<2> vf21(double x, Vec<2> vy)
{
    /*yの第1成分: y, 第2成分: ydot*/
    Vec<2> ret;
    ret[0] = vy[1]; //  dy/dx      = ydot
    ret[1] = vy[0]; //  d(ydot)/dx = y

    return ret;
}

/*RKnの引数の関数の例*/ // 2階微分方程式 d2y/dx2 = -y ---> 分解 --->  dy/dx = ydot,  d(ydot)/dx = -y 
Vec<2> vf22(double x, Vec<2> vy)
{
    /*yの第1成分: y, 第2成分: ydot*/
    Vec<2> ret;
    ret[0] = vy[1]; //  dy/dx      = ydot
    ret[1] = -vy[0]; //  d(ydot)/dx = -y

    return ret;
}


/*以下では、簡単な微分方程式を数値的に解き、解析解と比較する*/
#include <iomanip>
int main()
{
    {
        Vec<1> vy0;//初期条件
        Vec<1> sol;//数値解

        std::cout << std::fixed << std::setprecision(15);

        std::cout << "=========微分方程式1=========" << std::endl;
        std::cout << "y'(x)\t\t=" << "x*x*x*x" << std::endl << std::endl;

        vy0 = {0.0};
        std::cout << "---------境界条件---------" << std::endl;
        std::cout <<"y(0)\t\t=" << vy0[0] << std::endl << std::endl;

        sol = RKn<1>(0, 1, vy0, 0.001, vf11);
        std::cout << "---------数値解---------" << std::endl;
        std::cout <<"y(1)\t\t=" << sol[0] << std::endl << std::endl;

        std::cout << "---------解析解---------" << std::endl;
        std::cout <<"y(1)=1/5\t=" << 1.0/5 << std::endl << std::endl << std::endl;



        std::cout << "=========微分方程式2=========" << std::endl;
        std::cout << "y'(x)\t\t=" << "y(x)" << std::endl << std::endl;

        vy0 = {1.0};
        std::cout << "---------境界条件---------" << std::endl;
        std::cout <<"y(0)\t\t=" << vy0[0] << std::endl << std::endl;

        sol = RKn<1>(0, 1, vy0, 0.001, vf12);
        std::cout << "---------数値解---------" << std::endl;
        std::cout <<"y(1)\t\t=" << sol[0] << std::endl << std::endl;

        std::cout << "---------解析解---------" << std::endl;
        std::cout <<"y(1)=exp(1)\t=" << exp(1) << std::endl << std::endl << std::endl;
    }

    {
        Vec<2> vy0;
        Vec<2> sol;

        std::cout << "=========微分方程式3=========" << std::endl;
        std::cout << "y''(x)\t\t=" << "y(x)" << std::endl << std::endl;

        vy0 = {0.0, 1.0};
        std::cout << "---------境界条件---------" << std::endl;
        std::cout << "y(0)\t\t=" << vy0[0] << std::endl;
        std::cout <<"y\'(0)\t\t=" << vy0[1] << std::endl << std::endl;

        sol = RKn<2>(0, 1, vy0, 0.001, vf21);
        std::cout << "---------数値解---------" << std::endl;
        std::cout << "y(1)\t\t=" << sol[0] << std::endl;
        std::cout <<"y\'(1)\t\t=" << sol[1] << std::endl << std::endl;

        std::cout << "---------解析解---------" << std::endl;
        std::cout << "y(1)=sinh(1)\t=" << sinh(1) << std::endl;
        std::cout <<"y\'(1)=cosh(1)\t=" << cosh(1) << std::endl << std::endl << std::endl;


        std::cout << "=========微分方程式4=========" << std::endl;
        std::cout << "y''(x)\t\t=" << "-y(x)" << std::endl << std::endl;

        vy0 = {0.0, 1.0};
        std::cout << "---------境界条件---------" << std::endl;
        std::cout << "y(0)\t\t=" << vy0[0] << std::endl;
        std::cout <<"y\'(0)\t\t=" << vy0[1] << std::endl << std::endl;

        sol = RKn<2>(0, 1, vy0, 0.001, vf22);
        std::cout << "---------数値解---------" << std::endl;
        std::cout << "y(1)\t\t=" << sol[0] << std::endl;
        std::cout <<"y\'(1)\t\t=" << sol[1] << std::endl << std::endl;

        std::cout << "---------解析解---------" << std::endl;
        std::cout << "y(1)=sin(1)\t=" << sin(1) << std::endl;
        std::cout <<"y\'(1)=cos(1)\t=" << cos(1) << std::endl;
    }
}

