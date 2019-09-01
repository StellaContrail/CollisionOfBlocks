#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>
#define N pow(2, 30) // ������
#define OBSERVETIME 15 // �v������
#define COR 1 // �����W��
#include "unistd.h"

class Block {
private:
    struct positions {
        double left, right, center;
    };
public:
    Block(const double _x, const double _v, const double _width, const double _mass);
    double Kinetic(void);
    double width;
    double v;
    double x; // center pos
    double mass;
    positions GetPositions(void);
    bool IsColliding(Block block);
};

inline Block::Block(const double _x, const double _v, const double _width, const double _mass) {
    x = _x;
    v = _v;
    width = _width;
    mass = _mass;
}

inline double Block::Kinetic(void) {
    return 0.5 * mass * v * v;
}

inline Block::positions Block::GetPositions(void) {
    positions temp = {x - 0.5*width, x + 0.5*width, x};
    return temp;
}

inline bool Block::IsColliding(Block block) {
    positions pos = block.GetPositions();
    bool isRightSideEvaded = x - 0.5*width <= pos.left && pos.left <= x + 0.5*width;
    bool isLeftSideEvaded = x - 0.5*width <= pos.right && pos.right <= x + 0.5*width;
    bool isInside = pos.left <= x - 0.5*width && x + 0.5*width <= pos.right;
    if (isRightSideEvaded || isLeftSideEvaded || isInside) {
        return true;
    }
    else {
        return false;
    }
}


int main(void) {
    int digit = 4;
    Block a(5.0, 0.0, 2.5, 1); // Block1
    Block b(15.0, -2.0, 3.5, pow(100, digit)); // Block2
    Block Wall(-0.5, 0.0, 1.0, 0); // Wall
    double t = 0.0;
    const double dt = OBSERVETIME / (double)N;
    double total_energy = a.Kinetic() + b.Kinetic();
    double total_momentum = 0.0;
    double pre_relative_speed = 0.0;
    const double total_mass = a.mass + b.mass;
    bool collision_flag = false; // �Փ˂��Ă��甠����o��܂�true�ɂȂ� (�����߂��Ȃ��悤�ɂ��邽��)
    bool collision_flag2 = false;
    int count_num = 0;
    for (int i = 0; i < N; i++) {
        a.x += a.v * dt;
        b.x += b.v * dt;
        t += dt;

        if (a.IsColliding(Wall) && !collision_flag2) {
            std::cout << "collided with Wall" << std::endl;
            collision_flag2 = true;
            a.v = -a.v / (double)COR;
            count_num++;
        }
        if (!a.IsColliding(Wall) && collision_flag2) {
            collision_flag2 = false;
        }

        if (a.IsColliding(b) && !collision_flag) {
            std::cout << "collided with Block2" << std::endl;
            collision_flag = true;
            pre_relative_speed = a.v - b.v;
            total_momentum = a.mass * a.v + b.mass * b.v;
            a.v = (total_momentum - b.mass*pre_relative_speed/(double)COR) / total_mass;
            b.v = (total_momentum + a.mass*pre_relative_speed/(double)COR) / total_mass;
            count_num++;
        }
        if (!a.IsColliding(b) && collision_flag) {
            collision_flag = false;
        }
        
        //std::cout << "[" << std::fixed << std::showpos << t << "sec]\tPositions = (" << a.x << "," << b.x << ")" << "\tVelocity = (" << a.v << "," << b.v << ")" << std::endl;
        //usleep(dt * 1000000);
    }
    std::cout << "[" << std::fixed << std::showpos << t << "sec]\tPositions = (" << a.x << "," << b.x << ")" << "\tVelocity = (" << a.v << "," << b.v << ")" << std::endl;
    std::cout << count_num << std::endl;
    return 0;
}

