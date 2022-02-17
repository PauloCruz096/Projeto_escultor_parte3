#ifndef _ESCULTOR_H_
#define _ESCULTOR_H_
#include<iostream>
#include<vector>

using namespace std;

enum{XY, XZ, YZ};

struct Voxel{
    float r, g, b;
    float a;
    bool is0n;
};

class Sculptor{
protected:
    unsigned nx, ny, nz;
    float r, g, b, a;
    Voxel ***v;
public:
    Sculptor(unsigned _nx, unsigned _ny, unsigned _nz);
    ~Sculptor();
    void setColor(float _r, float _g, float _b, float alpha);
    void putVoxel(int x, int y, int z);
    void cutVoxel(int x, int y, int z);
    void putBox(int x0, int x1, int y0, int y1, int z0, int z1);
    void cutBox(int x0, int x1, int y0, int y1, int z0, int z1);
    void putSphere(int xcenter, int ycenter, int zcenter, int radius);
    void cutSphere(int xcenter, int ycenter, int zcenter, int radius);
    void putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
    void cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
    void writeOFF(const char* filename);
    vector<vector<Voxel>> getPlano(int i, int plano = XY);
};

#endif
