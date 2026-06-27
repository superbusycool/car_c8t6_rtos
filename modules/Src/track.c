#include "track.h"
#include "gpio.h"
#include "led.h"

//// 全局变量定义（需放在.c文件中，或在.h中用extern声明）
//volatile float track_status[1] = {0};
//volatile float track_status_backup[1][20] = {0};
//volatile uint32_t track_status_worse = 0;
//volatile _track_state track_state;
//
///***************************************************
//函数名: void track_check(void)
//说明:    5路循迹传感器状态检测与逻辑处理
//         1. 读取传感器状态并更新到位域
//         2. 计算循迹偏差值并备份历史状态
//         3. 根据传感器状态控制对应指示灯
//参数:    无
//返回:    无
//备注:    黑线检测为1，白底为0；偏差值：左偏为负，右偏为正，中间为0
//作者:    修改者
//****************************************************/
//void track_check(void)
//{
//    // 读取5路循迹传感器状态，更新到位域结构体
//    track_state.track.bit1 = GPIOGetValue(PORT2, 11);    // XJ5状态
//    track_state.track.bit2 = GPIOGetValue(PORT2, 8);    // XJ2状态
//    track_state.track.bit3 = GPIOGetValue(PORT2, 7);    // XJ1（中间）状态
//    track_state.track.bit4 = GPIOGetValue(PORT2, 9);   // XJ3状态
//    track_state.track.bit5 = GPIOGetValue(PORT2, 10);   // XJ4状态
//
//    // 备份历史状态（保留最近20次值）
//    for (uint16_t i = 19; i > 0; i--)
//    {
//        track_status_backup[0][i] = track_status_backup[0][i - 1];
//    }
//    track_status_backup[0][0] = track_status[0];  // 最新值存入备份首位置
//
//    // 更新track_state.state的值（将位域组合成一个字节）
//    // 假设bit1是最左传感器，bit5是最右传感器
//    track_state.state |= (track_state.track.bit1 & 0x01) << 0;  // bit1 -> 位0
//    track_state.state |= (track_state.track.bit2 & 0x01) << 1;  // bit2 -> 位1
//    track_state.state |= (track_state.track.bit3 & 0x01) << 2;  // bit3 -> 位2（中间）
//    track_state.state |= (track_state.track.bit4 & 0x01) << 3;  // bit4 -> 位3
//    track_state.state |= (track_state.track.bit5 & 0x01) << 4;  // bit5 -> 位4
//
//    // 根据传感器组合状态计算循迹偏差值
//    switch (track_state.state)
//    {
//        case 0x01:  // 00001：仅XJ1检测到黑线（最左）
//            track_status[0] = -4;
//            track_status_worse /= 2;  // 状态正常，减少异常计数
//            break;
//        case 0x03:  // 00011：XJ1、XJ2检测到黑线
//            track_status[0] = -3;
//            track_status_worse /= 2;
//            break;
//        case 0x02:  // 00010：仅XJ2检测到黑线
//            track_status[0] = -2;
//            track_status_worse /= 2;
//            break;
//        case 0x07:  // 00111：XJ1、XJ2、XJ3检测到黑线（左到中）
//            track_status[0] = -5;  // 宽范围左偏，取中间偏左值
//            track_status_worse /= 2;
//            break;
//        case 0x0F:  // 01111：XJ1、XJ2、XJ3、XJ4检测到黑线（左到中右）
//            track_status[0] = -6;  // 宽范围左偏，介于123和12之间
//            track_status_worse /= 2;
//            break;
//        case 0x06:  // 00110：XJ2、XJ3检测到黑线
//            track_status[0] = -1;
//            track_status_worse /= 2;
//            break;
//        case 0x04:  // 00100：仅XJ3检测到黑线（中间）
//            track_status[0] = 0;
//            track_status_worse /= 2;
//            break;
//        case 0x0C:  // 01100：XJ3、XJ4检测到黑线
//            track_status[0] = 1;
//            track_status_worse /= 2;
//            break;
//        case 0x08:  // 01000：仅XJ4检测到黑线
//            track_status[0] = 2;
//            track_status_worse /= 2;
//            break;
//        case 0x1E:  // 11110：XJ2、XJ3、XJ4、XJ5检测到黑线（中左到右）
//            track_status[0] = 6;  // 宽范围右偏，介于345和45之间
//            track_status_worse /= 2;
//            break;
//        case 0x1C:  // 11100：XJ3、XJ4、XJ5检测到黑线（中到右）
//            track_status[0] = 5;  // 宽范围右偏，取中间偏右值
//            track_status_worse /= 2;
//            break;
//        case 0x18:  // 11000：XJ4、XJ5检测到黑线
//            track_status[0] = 3;
//            track_status_worse /= 2;
//            break;
//        case 0x10:  // 10000：仅XJ5检测到黑线（最右）
//            track_status[0] = 4;
//            track_status_worse /= 2;
//            break;
//        case 0x1F:  // 11111：XJ1-XJ5全检测到黑线（全范围）
//            track_status[0] = 10;  // 视为居中（如宽黑线）
//            track_status_worse /= 2;
//            break;
//        case 0x00:  // 00000：无传感器检测到黑线（异常）
//            track_status[0] = track_status_backup[0][0];  // 沿用历史值
//            track_status_worse++;  // 增加异常计数
//            break;
//        default:    // 其他未定义状态（异常）
//            track_status[0] = track_status_backup[0][0];  // 沿用历史值
//            track_status_worse++;  // 增加异常计数
//            break;
//    }
//
//}
//

