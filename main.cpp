#include "tgaimage.h"
#include <cmath>  //使用round需要调用数学库
#include <cstdlib> // 这是给 rand() 和 srand() 用的底层工具箱
#include <ctime>   // 这是给 std::time() 用的时间工具箱
#include <iostream> // 用于在控制台输出文字和数字

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    

    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }


     if (ax>bx) { // make it left−to−right 起点坐标值大于终点坐标值 交换
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    
    for (int x=ax; x<=bx; x++) { //直接从ax指向bx 一格一格数
        float t = (x-ax) / static_cast<float>(bx-ax);
        int y = std::round( ay + (by-ay)*t );

        if (steep) // if transposed, de−transpose线放平了 当然要反着画
        framebuffer.set(y, x, color);
        else
        framebuffer.set(x, y, color);
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(std::time(0)); 

    // ⏱️ 探针 1：在循环开始前，记录 CPU 当前的时钟滴答数
    clock_t start_time = clock(); 

    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width;
        int ay = rand()%height;
        int bx = rand()%width;
        int by = rand()%height;
        
        line(ax, ay, bx, by, framebuffer, TGAColor{ 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), 
            255
        }); 
    }

    // ⏱️ 探针 2：循环结束，再次记录 CPU 的时钟滴答数
    clock_t end_time = clock(); 

    // ⏱️ 结算：计算差值，并把“滴答数”除以“每秒滴答频率”，换算成人类的秒数
    std::cout << "渲染耗时: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << " 秒\n";

    framebuffer.write_tga_file("framebuffer.tga");
    return 0; 
}