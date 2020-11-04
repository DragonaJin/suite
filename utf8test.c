#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
FILE *fp;
#define P2  2     
#define P3  3        //要求三三组合强度的参数个数
#define N  144       //测试用例全集个数  全部值组合
#define P  5         //参数个数
#define M  74        //
#define K  8
long cof_a = 5000;   //两种得分标准的权值
long cof_b = 5000;
long State = 0;
typedef struct
{
    unsigned int bit : 1; 
} sOriVect;
long vect[N][K];
long id[N];
long class_vect[K];
long VN[P] = {2, 3, 2, 3, 4};
long ID2[P2] = {0, 4};
long ID3[P3] = {1, 2, 3};
unsigned char test[N][P];
void TestSetup()
{
    long i, j, k, m;
    long n = 1;
    long nn;
    for(j=P-1; j>=0; j--)
    {
        nn = n;
        n = n*VN[j];
        for(k=0; k<VN[j]; k++)
        {
            for(i=k*nn; i<N; i=i+n)
            {
                for(m=0; m<nn; m++)
                {
                    test[i+m+0][j] = k+1;
                }
            }
        }
    }
}
void VectSetup()
{
    long i1, j1, i2, j2, i3, j3, n, m, k;
    long class_id = 0;
    for(n=0; n<N; n++)
    {
        id[n] = 1;
        m = 0;
        k = 0;
        for(i1=0; i1<2; i1++)
        {
            for(i2=0; i2<2; i2++)
            {
                if(i1 < i2)
                {
                    for(j1=1; j1<=VN[ID2[i1]]; j1++)
                    {
                        for(j2=1; j2<=VN[ID2[i2]]; j2++)
                        {
                            if(test[n][ID2[i1]] == j1 && test[n][ID2[i2]] == j2)
                            {
                                vect[n][k] = m;
                                k++;
                            }
                            m++;
                        }
                    }
                    if(n == 0)
                    {
                        class_vect[class_id] = m;
                        class_id++;
                    }
                }
            }
        }
        for(i1=0; i1<3; i1++)
        {
            for(i2=0; i2<3; i2++)
            {
                if(i1 < i2)
                {
                    for(i3=0; i3<3; i3++)
                    {
                        if(i2 < i3)
                        {
                            for(j1=1; j1<=VN[ID3[i1]]; j1++)
                            {
                                for(j2=1; j2<=VN[ID3[i2]]; j2++)
                                {
                                    for(j3=1; j3<=VN[ID3[i3]]; j3++)
                                    {
                                        if(test[n][ID3[i1]] == j1 && test[n][ID3[i2]] == j2 && test[n][ID3[i3]] == j3)
                                        {
                                            vect[n][k] = m;
                                            k++;
                                        }
                                        m++;
                                    }
                                }
                            }
                            if(n == 0)
                            {
                                class_vect[class_id] = m;
                                class_id++;
                            }
                        }
                    }
                }
            }
        }
        for(i1=0; i1<2; i1++)
        {
            for(i3=0; i3<3; i3++)
            {
                for(j1=1; j1<=VN[ID2[i1]]; j1++)
                {
                    for(j3=1; j3<=VN[ID3[i3]]; j3++)
                    {
                        if(test[n][ID2[i1]] == j1 && test[n][ID3[i3]] == j3)
                        {
                            vect[n][k] = m;
                            k++;
                        }
                        m++;
                    }
                }
                if(n == 0)
                {
                    class_vect[class_id] = m;
                    class_id++;
                }
            }
        }
    }
}
void VectToOriVect(sOriVect out_orivect[M], long vect_id)
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = 0;
    }
    for(i=0; i<K; i++)
    {
        out_orivect[vect[vect_id][i]].bit = 1;
    }
}
void SET_OriVect(sOriVect out_orivect[M], sOriVect in_orivect[M])
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = in_orivect[i].bit;
    }
}
void FAN_OriVect(sOriVect out_orivect[M], sOriVect in_orivect[M])
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = ~in_orivect[i].bit;
    }
}
void AND_OriVect(sOriVect out_orivect[M], sOriVect in_orivect1[M], sOriVect in_orivect2[M])
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = in_orivect1[i].bit & in_orivect2[i].bit;
    }
}
void OR_OriVect(sOriVect out_orivect[M], sOriVect in_orivect1[M], sOriVect in_orivect2[M])
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = in_orivect1[i].bit | in_orivect2[i].bit;
    }
}
void SUB_OriVect(sOriVect out_orivect[M], sOriVect in_orivect1[M], sOriVect in_orivect2[M])
{
    long i;
    for(i=0; i<M; i++)
    {
        out_orivect[i].bit = in_orivect1[i].bit & (~(in_orivect1[i].bit & in_orivect2[i].bit));
    }
}
long MOD_OriVect(sOriVect in_orivect[M])
{
    long i, mod = 0;
    for(i=0; i<M; i++)
    {
        mod = mod + in_orivect[i].bit;
    }
    return mod;
}
long MaxClass_OriVect(sOriVect in_orivect[M])
{
    int i, j, k, max_class = 0;
    for(i=0; i<class_vect[0]; i++)
    {
        if(in_orivect[i].bit)
        {
            max_class++;
        }
    }
    for(i=1; i<K; i++)
    {
        k = 0;
        for(j=class_vect[i-1]; j<class_vect[i]; j++)
        {
            if(in_orivect[j].bit)
            {
                k++;
            }
        }
        if(max_class < k)
        {
            max_class = k;
        }
    }
    return max_class;
}
void main()
{
    struct tm *oldtime, *newtime;
    long ltime;
    long i, j, k, n;
    sOriVect targ_vect[M] = {0};
    sOriVect fan_targ_vect[M], i_vect[M], i_and_fan_vect[M], sub_i_and_fan_vect[M];
    long f, g, h, hmax_id, hmax_cov;
    float fhmax, fh, ferr;
    fp = fopen("问题示例.out", "w");
    fprintf(fp,"2,+3,+2,+3,4\n\n");
    time( &ltime );
    oldtime = gmtime( &ltime );
    fprintf(fp,"基于集合运算的组合算法SP(a, b, c)\n");
    fprintf(fp,"  参数a = 0.5\n");
    fprintf(fp,"  参数b = 0.5\n");
    fprintf(fp,"  参数c = 2000000\n\n");
    fprintf(fp,"集合法计算开始时间：%s\n", asctime( oldtime ));
    printf("集合法计算开始时间：%s\n", asctime( oldtime ));
    printf("生成所有组合的测试用例...\n");
    TestSetup();
    printf("生成测试用例的标识向量...\n");
    VectSetup();
    printf("开始计算...\n");
Loop:
    hmax_cov = MOD_OriVect(targ_vect);
    printf("%d\n", M - hmax_cov);
    if(hmax_cov == M)
    {
        i = 0;
        for(j=0; j<N; j++)
        {
            if(id[j] == 0)
            {
                i++;
                printf("test[%d] = (", i);
                fprintf(fp,"test[%d] = (", i);
                for(k=0; k<P; k++)
                {
                    if(k == (P-1))
                    {
                        printf("%d)\n", test[j][k]);
                        fprintf(fp,"%d)\n", test[j][k]);
                    }
                    else
                    {
                        printf("%d, ", test[j][k]);
                        fprintf(fp,"%d, ", test[j][k]);
                    }
                }
            }
        }
        time( &ltime );
        newtime = gmtime( &ltime );
        fprintf(fp,"\n集合法计算结束时间：%s", asctime( newtime ));
        printf("\n集合法计算结束时间：%s", asctime( newtime ));
        if(0 == State) {
           fprintf(fp,"\n计算结束【完整运算】\n");
           printf("\n计算结束【完整运算】\n");
        } else {
           fprintf(fp,"\n计算结束【简化运算】\n");
           printf("\n计算结束【简化运算】\n");
        }
        fclose(fp);
        return;
    }
    fhmax = 0;
    hmax_id = -1;
    n = 0;
    FAN_OriVect(fan_targ_vect, targ_vect);
    for(i=0; i<N; i++)
    {
        if(id[i] == 1)
        {
            VectToOriVect(i_vect, i);
            AND_OriVect(i_and_fan_vect, i_vect, fan_targ_vect);
            f = MOD_OriVect(i_and_fan_vect);
            if(f > 0)
            {
                n = n + 1;
                if(n > 2000000) {
                    State = 1;
                    break;
                }
                SUB_OriVect(sub_i_and_fan_vect, fan_targ_vect, i_and_fan_vect);
                g = MaxClass_OriVect(sub_i_and_fan_vect);
                h = cof_a*f + cof_b*(M - g);
                fh = (cof_a*f + cof_b*(M - g))/10000.;
                ferr = fh - fhmax;   //这里巧妙地利用了计算机内部处理数据的方式引入了一个随机因子
                if(ferr > 0)
                {
                    fhmax = fh;
                    hmax_id = i;
                }
            }
        }
    }
    id[hmax_id] = 0;
    VectToOriVect(i_vect, hmax_id);
    OR_OriVect(targ_vect, i_vect, targ_vect);
    goto Loop;
}
