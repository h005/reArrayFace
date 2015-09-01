#ifndef UFFACE_H
#define UFFACE_H

#include <iostream>
#include <set>
#include <vector>

class QString;

/*
 * 由于OpenMesh读入的面都需要是同一个方向的（顺时针或逆时针），
 * 所以需要解决.off文件中面的绕序问题，使它们的绕序相同
 *
 * 假设有N个面
 * 这个问题至少要做N*(N+1)/2比较，确定每两个面之间的绕序关系是否相同
 *
 * 步骤：
 * 1、检测每两个面之间的绕序关系，并设置到矩阵中。
 * 2、使用并查集，具有相同绕序的面合并
 * 3、将其它不同顺序的面的顺序调节过来（只需要转换第一个和第三个顶点的顺序即可）
 * 4、将结果输出到文件中。
 *
 *
 */

class UFface
{
public:
    UFface();
    UFface(QString path);
    void readin(QString path);

    int find(int p);
    void unionFace(int p,int q);

    int findCommonEdge(int p);
    void unionFaceCommonEdge(int p,int q);

    void unionFinal();
    //存储不同面的不同类别
    void setCateSet();
    //存储有公共边的不同面的不同类别
    void setCateCommonEdgeSet();
    //重新修改面的顶点顺序
    void reArrange();
    //输出到文件中，命名方式为源文件名+'s'+后缀
    void printOut(QString fileName);
private:
    //设置每两个面之间的二元关系，检测是否属于同一个类别
    void setRelation();
    //检测两个面是否有公共的边，这两个面是否绕向相同
    void checkIn(int i,int j);


private:
    std::set<int> cateSet;
    std::set<int> cateSetCommonEdge;

    std::vector<int> *cate;

    int *sz;
    int *szCE;
    //存储面的顶点绕序
    int **arrayFace;
    int *id;
    int *idCE;
    int NUM_FACE;
    char **relationGraph;
};

#endif // UFFACE_H
