#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;



int main() {
    // 读取图像文件
    ifstream infile("image.rgb", ios::in | ios::binary);
    if (!infile.is_open()) {
        cout << "无法打开图像文件" << endl;
        return -1;
    }
    
    // 图像的尺寸
    int width = 640;
    int height = 480;

    // 逐像素处理
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 读取像素
            unsigned char r, g, b;
            infile.read(reinterpret_cast<char*>(&r), sizeof(r));
            infile.read(reinterpret_cast<char*>(&g), sizeof(g));
            infile.read(reinterpret_cast<char*>(&b), sizeof(b));
            
            // 判断红色强度是否大于绿色和蓝色
            if (r > max(g, b)) {
                // 红色像素
                // 可以在这里做一些处理，比如标记该像素为红色
            }
        }
    }
    
    // 关闭文件
    infile.close();
    
    return 0;
}