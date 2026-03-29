day1
完成编译器的安装以及相关问题的解决
关于 PATH（系统坐标）：
你说得很对，它就是个坐标。但更深层的逻辑是：Windows 是个懒惰的管家。

如果你不把 bin 文件夹放进 PATH，你每次喊“g++”，管家都会摊摊手说 ObjectNotFound（我不认识这位巫师）。

你修改 PATH，本质上是给了管家一份**“常驻联络清单”**。从此以后，只要你喊出名字，管家就会自动顺着清单上的地址去帮你请出那位巫师。

关于 CTRL+S（虚影落纸）：
这个比喻你抓得最准！请记住这个底层真相：

编辑器（VS Code）：它是魔法师的**“意念”**，存在于易失的内存（RAM）中。

编译器（g++）：它是严谨的**“法律执行司”**，它只认刻在岩石（硬盘/SSD）上的文字。

保存（Save）：就是意念向物质的转化。不保存，你的代码就是一团随时会消散的烟雾。


 TGAImage framebuffer(width, height, TGAImage::RGB);
 类的定义



 你刚才总结的“指明类、指明对象、指明实现”，在计算机科学里有非常标准的对应关系：

类（Class - TGAImage）：这是你的“魔法图纸”。它规定了一张图片应该具备哪些属性（宽、高、像素数据）以及它能做什么（比如 set 一个像素，或者保存成文件）。

对象（Object - framebuffer）：这是根据图纸造出来的“实物”。你在内存里开辟了一块具体的空间。

构造函数参数（width, height, RGB）：这是造实物时的“定制要求”。

width 和 height 决定了矩阵的大小。

TGAImage::RGB 是一个模式开关。它告诉对象：每个格子（像素）里要塞进 3 字节的数据（红、绿、蓝）。如果是 GRAYSCALE，那每个格子就只塞 1 字节（黑白灰）。


3.16 2026
C++先编译再执行
编译方法 在powershell上 用
g++ 呼叫C++编译器
 g++  main.cpp tgaimage.cpp -o main.exe

 它的左括号 { 和右括号 } 必须严格一一对应。
 记得保存代码  
 线断点太多
 t+=.02取太少  50个点







 3.17
 line(cx, cy, ax, ay, framebuffer, yellow);
    line(ax, ay, cx, cy, framebuffer, red);
    压力测试
    如果算法完美 那么就看不到红色的线

    cd /d/projects-gp/tinyrenderer-me
    g++ main.cpp tgaimage.cpp -o main.exe
    ./main.exe
    运行结果是三条不连续的线
    当前line函数

    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    for (float t=0.; t<1.; t+=.00001) {  // t就是 段的大小 改t值没用 ax 和  bx分别是起始和终止横坐标 同理bx by是纵向的
        int x = std::round( ax + (bx-ax)*t );
        int y = std::round( ay + (by-ay)*t );//&引用符号
        framebuffer.set(x, y, color); // 漏了这最关键的一句：下笔涂色！


    }


    关键问题解决逻辑
确定“主导权” (找老大)：对比 $\Delta x$ 和 $\Delta y$ 的绝对值，谁大谁就是主轴。在你的例子里，$\Delta x = 10$，$\Delta y = 2$，所以 $X$ 轴是老大，总共要取 10 个像素点。分配“步幅” (定步长)：为了保证连续不断点，老大 $X$ 轴必须老老实实地每步走 1 格。而小弟 $Y$ 轴必须配合老大，把它的总路程均摊到这 10 步里。所以 $Y$ 轴每步只走 $\frac{2}{10} = 0.2$ 格。积累与取整 (凑出 10 个点)：老大走 1 步，小弟走 0.2 步；老大走 2 步，小弟走 0.4 步……因为屏幕上只有整数格子，所以电脑在画图时会四舍五入。这样 $Y$ 坐标会在前几步停留在同一个像素高度，积累到 0.5 以上时再跳一格。最终完美且连续地凑出 10 个坐标对！


void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    
     if (ax>bx) { // make it left−to−right 起点坐标值大于终点坐标值 交换
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    
    for (int x=ax; x<=bx; x++) { //直接从ax指向bx 一格一格数
        float t = (x-ax) / static_cast<float>(bx-ax);
        int y = std::round( ay + (by-ay)*t );
        framebuffer.set(x, y, color);
    }
    出现bug  绿色线消失
    原因  绿色线的终点坐标值 小于起点坐标值  没有满足for循环条件
    
    加上这一段：
     if (ax>bx) { // make it left−to−right 起点坐标值大于终点坐标值 交换
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    解决

    蓝线又高又窄
    所以没有显示
    蓝线（从 a 到 b）：起点是 (7, 3)，终点是 (12, 37)。

X 轴总共只需要走 5 步 (从 7 到 12)。

Y 轴却需要爬升 34 步 (从 3 到 37)。


std::swap的原理



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
3.18
for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width;
        int ay = rand()%height;
        int bx = rand()%width;
        int by = rand()%height;
        } // ⬅️ 🚨🚨🚨 案发第一现场：就是这个右大括号！🚨🚨🚨

        // ---------------------------------------------------------
        // ！！！从这条虚线往下，ax, ay, bx, by 在内存里已经被彻底清空！！！
        // ---------------------------------------------------------

        // 所以，当代码执行到下面这行时，编译器发现你在呼叫几个“死人”，直接给你标红报错：
        line(ax, ay, bx, by, framebuffer, TGAColor{


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(std::time(0)); 
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width;
        int ay = rand()%height;
        int bx = rand()%width;
        int by = rand()%height;
        
        // 【修复原因 1】：移除了原本在这里的 '}'！
        // 把 line 函数强行包进 for 循环的密室里。
        // 这样不仅 line 能认识活着的 ax, ay...，而且才能随着 for 循环执行几百万次。
        line(ax, ay, bx, by, framebuffer, TGAColor{ 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), // 安全降维
            255
        }); 
    } // 【修复原因 2】：for 循环的右大括号被移到了这里！在这里正式结束一次循环。

    framebuffer.write_tga_file("framebuffer.tga");


for 循环那 1600 万次画线究竟消耗了多少纯粹的 CPU 滴答数。

g++ main.cpp tgaimage.cpp -O3 -o main.exe
O3的作用
我不管你函数是什么，我就直接把代码顺序铺在 for 循环里”


所以下一个问题做个笔记啊，就是说这个问题的话，就是为什么现在还是12秒，是因为这个float X, 也就是这段代码，我们是用于去计算，就是当前的这个X上跑的这个点。它跑到这个点跑到这个位置的时候，它和起点之间的这个与站总长的一个比例的这个T。这个T的话，我们之后要用到Y轴上啊，因为因为这是在窄线当中去用的嘛，就如果说这个不不是的，是的你就他就去用嘛，所以说你在你说在这个地方为什么慢呢？因为这个地方用的是除法，除法要用十几个甚至几十个机器周期，它是加法和减法的几十倍十几十几倍几十倍，那浮点数的话二是浮点数的话，带这些小数点的东西呢，它是被拆成符号位，按指位数和位数位位数位来存的，所以说要先把它对齐再算，所以说就会很慢，嗯让他做他最讨厌最不擅长的事情，黑奴啊，操

. 物理层面的“跨部门沟通成本”（ALU vs FPU）
在 CPU 的主板上，处理整数加减法的地方叫 ALU（算术逻辑单元），这是 CPU 最核心、最快、离寄存器最近的亲儿子。
而处理浮点数的地方叫 FPU（浮点运算单元）。
当你写下那句 float t = ... 时，CPU 的主流水线必须停下来，把数据打包，通过总线扔给旁边的 FPU，FPU 算完之后，还要把结果扔回给 ALU 去做后续处理。这种硬件级别的“跨部门数据交接”，极大地拖慢了那 1600 万次循环。

2. IEEE 754 协议的拆包与打包
正如你总结的，浮点数在内存里不是直接存数字的，它是存了一套“科学记数法”的公式。

每次做除法，FPU 都要执行极其繁琐的步骤：

提取：把 32 位数据强行切开，拿出 1 位符号位、8 位指数位、23 位尾数位。

对阶：把两个数字的指数强行拨到同一个量级。

计算：尾数相除。

规格化：算完之后，如果小数点跑偏了，还得重新移动小数点，修改指数位。

舍入：最后还要处理精度丢失（比如你用的 std::round）。



像素的物理本质
我大概懂了，也就是说，所谓像素呢，就是这个led矩阵电路板上的分割的一个单位。当然，我可以任意分割是吧，但它每一个分割的，它的方块也就是它长宽高都是均匀的标准化的，也就是他们都是相差不大大差，他们都是完全一致的，才能可以做每个方块里面有3个二极管。这个二极管呢，就是每个方块有3个二极管，所以说让我去画划线。本质上呢，就是说坐标就是这个像素的索引

像素在内存里是用数组（Array）存起来的。坐标 $(x, y)$ 本质上就是数组的索引（Index/门牌号）

float t = (x-ax) / static_cast<float>(bx-ax);
        int y = std::round( ay + (by-ay)*t );



        void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    

    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }

3.19
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
}代码升级



g++ main.cpp tgaimage.cpp -O3 -o main.exe



为了解决 性能瓶颈

误差累积法
int y = ay;
    float error = 0;
    for (int x=ax; x<=bx; x++) { //直接从ax指向bx 一格一格数
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);//线放平了 当然要反着画
        else
            framebuffer.set(x, y, color);
        error += std::abs(by-ay)/static_cast<float>(bx-ax);//绝对值的含义  $\tan(-\theta) = -\tan(\theta)$
        if (error>.5) {
            y += by > ay ? 1 : -1;
            error -= 1.;
消灭除法的方法 Bresenham算法
void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
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
        if (ierror > bx - ax) {
            y += by > ay ? 1 : -1;
            ierror -= 2 * (bx-ax);
        }
    }
}
            由Bresenham算法得出的采用C语言的带有布尔值的乘法解决分支带来的问题

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


        