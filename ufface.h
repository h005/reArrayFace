#ifndef UFFACE_H
#define UFFACE_H

#include <iostream>
#include <set>

class QString;

class UFface
{
public:
    UFface();
    UFface(QString path);
    void readin(QString path);
    int find(int p);
    void unionFace(int p,int q);
    void unionFinal();
    void setCateSet();
    void reArrange();
    void printOut(QString fileName);
private:
    void setRelation();
    void checkIn(int i,int j);


private:
    std::set<int> cateSet;
    int *sz;
    int **arrayFace;
    int *id;
    int NUM_FACE;
    char **relationGraph;
};

#endif // UFFACE_H
