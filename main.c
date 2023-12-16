#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "gravity.h" // この中に構造体定義、関数プロトタイプがある

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);

int main(int argc, char **argv) {
    const Condition cond =
        {
            .width  = 75,
            .height = 40,
            .G = 1.0,
            .dt = 1.0
        };
    
    size_t objnum = 2;
    Object objects[objnum];
    
    // オブジェクトの定義 
    objects[0] = (Object){ .m = 10.0, .y = -20.0, .vy = 0.3};
    objects[1] = (Object){ .m = 10.0, .y =  20.0, .vy = 0.0};
    
    // シミュレーション. ループは整数で回しつつ、実数時間も更新する
    const double stop_time = 400;
    double t = 0;
    for (size_t i = 0 ; t <= stop_time ; i++){
        t = i * cond.dt;
        my_update_velocities(objects, objnum, cond);
        my_update_positions(objects, objnum, cond);
        
        // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
        my_plot_objects(objects, objnum, t, cond);
        
        usleep(500 * 1000); // 200 x 1000us = 200 ms ずつ停止
        printf("\e[%dA", cond.height+2);// 表示位置を巻き戻す。壁がないのでheight+2行（境界とパラメータ表示分）
    }
    return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的には physics.h 内の、こちらが用意したプロトタイプをコメントアウト
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
    printf("\n");
    int arr[numobj];
    for (int i = 0; i < numobj; i++) {
        arr[i] = (int)objs[i].y;
    }

    for(int y = 0; y < cond.height; y++){
        for (int x = 0; x < cond.width; x++) {
            if (x == cond.width/2) {
                for (int i = 0; i < numobj; i++) {
                    if (y == cond.height/2 + arr[i] - 1) {
                        printf("o");
                    }
                }
            }
            printf(" ");
        }
        printf("\n");
    }

    printf("-----\n");
    printf("t = %f", t);
    for (int i = 0; i < numobj; i++) {
        printf(", objs[%d].y = %f", i, objs[i].y);
    }
}


void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i < numobj; i++) {
        double acc = 0;
        for (int j = 0; j < numobj; j++) {
            if (i == j) continue;
            acc += cond.G * objs[j].m * (objs[j].y - objs[i].y) / fabs(pow(objs[i].y - objs[j].y, 3));
        }
        objs[i].prev_vy = objs[i].vy;
        objs[i].vy += acc * cond.dt; 
    }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for (int i = 0; i < numobj; i++) {
        objs[i].y += objs[i].prev_vy  * cond.dt;
    } 
}