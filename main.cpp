#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define NDX 128
#define NDY 128

int ndmx = NDX - 1;
int ndmy = NDY - 1;

int step = 0;
int nstep = 20001;
int pstep = 100;

double dx = 1.;
double dtime = 1.e-2;
double Da = 0.4;
double Db = 6.2;
double p = 0.;
double q = 1.6;
double r = 0.5;

int i, j, ip, im, jp, jm;
double a[NDX][NDY], b[NDX][NDY], a2[NDX][NDY], b2[NDX][NDY];
double dadt, dbdt;
double fae, faw, fan, fas, fbe, fbw, fbn, fbs;

void init_cond(){
    for (i = 0; i <= ndmx; i++){
        for (j = 0; j <= ndmy; j++){
            a[i][j] = (rand() + 0.5)/(1. + RAND_MAX);
            b[i][j] = 1. - a[i][j];
        }
    }
}

double funcf(double aaa, double bbb){
    double resf;
    resf = p - q * aaa + (aaa * aaa) / (bbb * (1. + r * aaa * aaa));
    return resf;
}

double funcg(double aaa, double bbb){
    return aaa * aaa - bbb;
}

void datasave(int step){
    
    FILE *stream; //ストリームのポインタ設定
    char buffer[30];
    sprintf(buffer, "data1/2d%d.csv", step);
    stream = fopen(buffer, "w"); //書き込む先のファイルを追記方式でオープン

    for (int i = 0; i <= ndmx; i++){
        for (j=0; j<=ndmy; j++){
            fprintf(stream, "%e   ", a[i][j]);
            fprintf(stream, "\n");
        }
    }
    fclose(stream); //ファイルをクローズ

    FILE *streamc; //ストリームのポインタ設定
    char bufferc[30];
    sprintf(bufferc, "data2/2d%d.csv", step);
    streamc = fopen(bufferc, "w"); //書き込む先のファイルを追記方式でオープン

    for (int i = 0; i <= ndmx; i++){
        for (j=0; j<=ndmy; j++){
            fprintf(streamc, "%e   ", b[i][j]);
            fprintf(streamc, "\n");
        }
    }
    fclose(streamc); //ファイルをクローズ
}

int main(void)
{
    init_cond();

start:;
    if ((((int)(step) % pstep) == 0))
    {
        datasave(step);
        cout << step << " steps(" << step * dtime << " seconds) has done!" << endl;
    }

    for (i=0; i<=ndmx; i++){
        for (j=0; j<=ndmy; j++){
            ip = i + 1;
            im = i - 1;
            jp = j + 1;
            jm = j - 1;
            if (i == ndmx){ip = ndmx;}
            if (i == 0){im = 0;}
            if (j == ndmy){jp = 0;}
            if (j == 0){jm = ndmy;}
            fae = Da * (a[ip][j] - a[i][j])/dx;
            faw = Da * (a[i][j] - a[im][j])/dx;
            fan = Da * (a[i][jp] - a[i][j])/dx;
            fas = Da * (a[i][j] - a[i][jm])/dx;
            fbe = Db * (b[ip][j] - b[i][j])/dx;
            fbw = Db * (b[i][j] - b[im][j])/dx;
            fbn = Db * (b[i][jp] - b[i][j])/dx;
            fbs = Db * (b[i][j] - b[i][jm])/dx;
            
            dadt = (fae - faw + fan - fas) / dx + funcf(a[i][j], b[i][j]);
            dbdt = (fbe - fbw + fbn - fbs) / dx + funcg(a[i][j], b[i][j]);

            a2[i][j] = a[i][j] + dadt * dtime;
            b2[i][j] = b[i][j] + dbdt * dtime;
        }
    }

    for (i = 0; i <= ndmx; i++){
        for (j = 0; j <= ndmy; j++){
            a[i][j] = a2[i][j];
            b[i][j] = b2[i][j];
        }
    }

    step = step + 1;
    if (step < nstep)
    {
        goto start;
    }

end:;
    return 0;
}