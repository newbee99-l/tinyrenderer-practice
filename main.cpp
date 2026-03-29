#include "tgaimage.h"
#include <cmath>  //使用round需要调用数学库
#include <cstdlib> // 这是给 rand() 和 srand() 用的底层工具箱
#include <ctime>   // 这是给 std::time() 用的时间工具箱
#include <iostream> // 用于在控制台输出文字和数字
#include "model.h"
#include "geometry.h" // 顺手把数学库也接通，一会存坐标要用

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
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }
}

int main(int argc, char** argv) {
   // 🪟 第一步：扩大画布！
    // 昨天测性能用的是 64x64，现在要画一个精密的人头，画布太小会变成马赛克！
    // 我们直接升级到 800x800 的高清物理面板！
    constexpr int width  = 800;
    constexpr int height = 800;
    TGAImage framebuffer(width, height, TGAImage::RGB);
    // 💾 第二步：硬盘数据载入内存！
    // 调用作者写的引擎，把目录下的 obj 文件生吞进去
    Model *model = new Model("african_head.obj");
   