//
// Created by SuperChen on 2026/6/20.
//

#ifndef CAR_PROJECT1_CONFIG_H
#define CAR_PROJECT1_CONFIG_H

//电机最大速度约为6.4r/s
#include "pid.h"

#define Motor0Speed_KP_V         40
#define Motor0Speed_KI_V         35
#define Motor0Speed_KD_V         0
#define Motor0Speed_INTEGRAL_V   70
#define Motor0Speed_MAX_V        100

#define Motor1Speed_KP_V         40
#define Motor1Speed_KI_V         35
#define Motor1Speed_KD_V         0
#define Motor1Speed_INTEGRAL_V   70
#define Motor1Speed_MAX_V        100

#define CarTurn_KP_V         4.5f
#define CarTurn_KI_V         0.01f
#define CarTurn_KD_V         0.0001f
#define CarTurn_INTEGRAL_V   10
#define CarTurn_MAX_V        40

#define CarSelfTurn_KP_A         0.5
#define CarSelfTurn_KI_A         0
#define CarSelfTurn_KD_A         0.002f
#define CarSelfTurn_INTEGRAL_A   0
#define CarSelfTurn_MAX_A        3

#define eightline_search_KP         1.1f
#define eightline_search_KI         0
#define eightline_search_KD         0.001f
#define eightline_search_INTEGRAL   0    //积分项
#define eightline_search_MAX        2    //最大输出值



#define SR04_TIMEOUT_US   30000  /* 超时时间 (us), 对应 ~5m         */
#define SR04_MAX_DIST_CM  400    /* 最大有效距离 (cm)               */


#endif //CAR_PROJECT1_CONFIG_H
