#include "ufface.h"

#include <QString>

UFface::UFface()
{

}

UFface::UFface(QString path)
{
    readin(path);
    std::cout<<"read in done..."<<std::endl;
    sz = new int[NUM_FACE];
    memset(sz,0,sizeof(int)*NUM_FACE);
    szCE = new int[NUM_FACE];
    memset(szCE,0,sizeof(int)*NUM_FACE);
    id = new int[NUM_FACE];
    idCE = new int[NUM_FACE];
    for(int i=0;i<NUM_FACE;i++)
        id[i] = idCE[i] = i;
    relationGraph = new char*[NUM_FACE];
    for(int i=0;i<NUM_FACE;i++)
    {
        relationGraph[i] = new char[NUM_FACE];
        memset(relationGraph[i],0,sizeof(char)*NUM_FACE);
    }
}

void UFface::readin(QString fileName)
{
    freopen(fileName.toStdString().c_str(),"r",stdin);
    int numVer,numFace,numEdge,tmp;
    char ss[80];
    // read OFF
    gets(ss);
    // read numVer,numFace,numEdge
    scanf("%d %d %d\n",&numVer,&numFace,&numEdge);
    // read vertices
    for(int i=0;i<numVer;i++)
        gets(ss);
    // read faces
    arrayFace = new int*[numFace];
    for(int i=0;i<numFace;i++)
    {
        arrayFace[i] = new int[3];
        scanf("%d %d %d %d\n",&tmp,&arrayFace[i][0],&arrayFace[i][1],&arrayFace[i][2]);
    }
    fclose(stdin);
    NUM_FACE = numFace;
}

int UFface::find(int p)
{
    while(p != id[p])
    {
        id[p] = id[id[p]];
        p = id[p];
    }
    return p;
}

void UFface::unionFace(int p, int q)
{
    int fp = find(p);
    int fq = find(q);
    if(fp == fq)
        return;
    if(sz[fp] > sz[fq])
    {
        id[fq] = fp;
        sz[fp] += sz[fq];
    }
    else
    {
        id[fp] = fq;
        sz[fq] += sz[fp];
    }
}

int UFface::findCommonEdge(int p)
{
    while(p != idCE[p])
    {
        idCE[p] = idCE[idCE[p]];
        p = idCE[p];
    }
    return p;
}

void UFface::unionFaceCommonEdge(int p, int q)
{
    int fp = findCommonEdge(p);
    int fq = findCommonEdge(q);
    if(fp == fq)
        return;
    if(szCE[fp] > szCE[fq])
    {
        idCE[fq] = fp;
        szCE[fp] += szCE[fq];
    }
    else
    {
        idCE[fp] = fq;
        szCE[fq] += szCE[fp];
    }
}

void UFface::unionFinal()
{
    setRelation();
    for(int i=1;i<NUM_FACE;i++)
        for(int j=0;j<i;j++)
        {
            if(relationGraph[i][j]%2==1)
                unionFace(i,j);
            //有公共边
            if((relationGraph[i][j]>>1)%2==1)
                unionFaceCommonEdge(i,j);
        }
    setCateCommonEdgeSet();
    setCateSet();
    printf("unionFinal... num of Categories: %d\n",cateSet.size());
    printf("unionFinal... num of CategoriesCommonEdge: %d\n",cateSetCommonEdge.size());

    if(cateSet.size()==cateSetCommonEdge.size())
        return;
    do{
        setRelation();
        for(int i=1;i<NUM_FACE;i++)
            for(int j=0;j<i;j++)
                if(relationGraph[i][j]%2==1)
                    unionFace(i,j);
        setCateSet();
        printf("unionFinal... num of Categories: %d\n",cateSet.size());
        printf("unionFinal... num of CategoriesCommonEdge: %d\n",cateSetCommonEdge.size());
        if(cateSet.size() == cateSetCommonEdge.size())
            return;
        reArrange();
//        getchar();
    }while(cateSet.size() != cateSetCommonEdge.size());
}

void UFface::setCateSet()
{
    cateSet.clear();
    for(int i=0;i<NUM_FACE;i++)
        cateSet.insert(find(i));
}

void UFface::setCateCommonEdgeSet()
{
    cateSetCommonEdge.clear();
    for(int i=0;i<NUM_FACE;i++)
        cateSetCommonEdge.insert(findCommonEdge(i));
}

void UFface::reArrange()
{
    cate = new std::vector<int>[cateSetCommonEdge.size()];
    std::set<int>::iterator it = cateSetCommonEdge.begin();
    int index = 0;
    for(;it!=cateSetCommonEdge.end();it++,index++)
        for(int i=0;i<NUM_FACE;i++)
            if(findCommonEdge(i)==*it)
                cate[index].push_back(i);

    for(int i=0;i < cateSetCommonEdge.size();i++)
    {
        int pid = find(cate[i][0]);
        for(int j=1; j < cate[i].size();j++)
        {
            if(find(cate[i][j]) != pid)
            {
                int tmp = arrayFace[cate[i][j]][0];
                arrayFace[cate[i][j]][0] = arrayFace[cate[i][j]][2];
                arrayFace[cate[i][j]][2] = tmp;
            }
        }
    }

}

void UFface::printOut(QString fileName)
{
    freopen(fileName.toStdString().c_str(),"r",stdin);
    int len = fileName.length();
    fileName.insert(len-4,"s");
    freopen(fileName.toStdString().c_str(),"w",stdout);
    char ss[80];
    int numVer,numFace,numEdge;
    gets(ss);
    puts(ss);
    scanf("%d %d %d\n",&numVer,&numFace,&numEdge);
    printf("%d %d %d\n",numVer,numFace,numEdge);
    for(int i=0;i<numVer;i++)
    {
        gets(ss);
        puts(ss);
    }
    for(int i=0;i<numFace;i++)
        printf("3 %d %d %d\n",arrayFace[i][0],arrayFace[i][1],arrayFace[i][2]);
    fclose(stdin);
    fclose(stdout);
    freopen("CON","w",stdout);
}

void UFface::setRelation()
{
    for(int i=1;i<NUM_FACE;i++)
        for(int j=0;j<i;j++)
            checkIn(i,j);
}

void UFface::checkIn(int i, int j)
{
    unsigned long long num0[3]={
        (unsigned long long)arrayFace[i][0]<<32 | (unsigned long long)arrayFace[i][1],
        (unsigned long long)arrayFace[i][1]<<32 | (unsigned long long)arrayFace[i][2],
        (unsigned long long)arrayFace[i][2]<<32 | (unsigned long long)arrayFace[i][0]
    };
    unsigned long long num1[3]={
        (unsigned long long)arrayFace[j][1]<<32 | (unsigned long long)arrayFace[j][0],
        (unsigned long long)arrayFace[j][2]<<32 | (unsigned long long)arrayFace[j][1],
        (unsigned long long)arrayFace[j][0]<<32 | (unsigned long long)arrayFace[j][2]
    };
    unsigned long long num2[3]={
        (unsigned long long)arrayFace[j][0]<<32 | (unsigned long long)arrayFace[j][1],
        (unsigned long long)arrayFace[j][1]<<32 | (unsigned long long)arrayFace[j][2],
        (unsigned long long)arrayFace[j][2]<<32 | (unsigned long long)arrayFace[j][0]
    };
    for(int i0 = 0; i0 < 3; i0++)
        for(int j0 = 0; j0 < 3 ; j0++)
        {
            if(num0[i0]==num1[j0])
                relationGraph[i][j] += 3;
            if(num0[i0]==num2[j0])
                relationGraph[i][j] += 2;
        }
}

