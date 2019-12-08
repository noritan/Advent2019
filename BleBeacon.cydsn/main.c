/* ========================================
 *
 * Copyright noritan.org, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF noritan.org.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

// Major/Minor フィールドの位置
#define     MAJOR_OFFSET    (25u)
#define     ID_OFFSET       (MAJOR_OFFSET+1)
#define     MINOR_OFFSET    (27u)
#define     HUM_OFFSET      (MINOR_OFFSET)
#define     TEMP_OFFSET     (MINOR_OFFSET+1)

// Temp フィールドの最小・最大値
#define     TEMP_ZERO       (69u)   //  0℃相当の値
#define     TEMP_40         (126u)  // 40℃相当の値

// Minor フィールドの最小・最大値
#define     HUM_ZERO        (13u)   //  0%相当の値
#define     HUM_50          (114u)  // 50%相当の値

// BLE パケット送信周期
#define     PACKET_PERIOD   (14)    // 1.5s * 14 = 21s

// Major/Minor フィールドの値
uint8   f_temp = TEMP_ZERO;         // Minor:tempフィールドの値
uint8   f_hum = HUM_ZERO;           // Minor:humフィールドの値
uint8   f_id = 0;                   // Major:IDフィールドの値

// BLE の初期化終了フラグ
uint8   initialized = 0;

// ソフトウェアタイマのカウンタ
uint8   tick = 0;

// ステートマシンイベントフラグ
uint32  wdt_Flag = 0;

// 500msごとに周期割り込みをかける
void Wdt_Callback(void) {
    wdt_Flag = 1u;
}

// BLE スタックの処理ハンドラ
void StackEventHandler(uint32 event, void *eventParam) {
    switch (event) {
        // スタックが立ちあがったまたはデバイスが切断されたら初期化終了とみなす
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            initialized = 1;
            break;
        // それ以外のイベントは無視する
        default:
            break;
    }
}

// ステートマシンの状態コード
enum StateCode {
    ST_INIT = 0,
    ST_FORCE,
    ST_CALCULATE,
    ST_SEND,
    ST_STOP,
    ST_SHOW
};

int main(void) {
    CYBLE_API_RESULT_T apiResult;       // エラーコード
    enum StateCode state = ST_INIT;
    double temp;
    double pressure;
    char sbuf[64];

    // 割り込みを許可する
    CyGlobalIntEnable;
    
    // I2C インターフェイスを起動する
    I2CM_Start();
    
    // LCD 表示を初期化する
    I2C_LCD_Start();
    
    // バックライトを初期化する
    I2C_LED_Start();
    I2C_LED_WritePWM(0x33, 0x33, 0x88, 0x00);

    // 圧力センサを初期化する
    I2C_SENSOR_Start();
    
    // 低速タイマ Timer0 のコールバック関数を設定する
    CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0, Wdt_Callback);

    // BLE デバイスの初期化を行う
    apiResult = CyBle_Start(StackEventHandler);
    // 初期化が正常に終わったかを確認する
    CYASSERT(apiResult == CYBLE_ERROR_OK);

    for(;;) {
        // ステートマシンのディスパッチャは500msごとに動作する
        if (wdt_Flag) {
            wdt_Flag = 0;

            // ステートマシン
            switch (state) {
                case ST_INIT:
                    // BLEシステムの初期化待ち
                    if (initialized) {
                        state = ST_FORCE;
                    }
                    break;
                case ST_FORCE:
                    // 温度と気圧の測定開始
                    I2C_SENSOR_Force();
                    state = ST_CALCULATE;
                    break;
                case ST_CALCULATE:
                    // 温度と気圧を計算する
                    I2C_SENSOR_ReadRawData();
                    temp = I2C_SENSOR_GetTemperature();
                    pressure = I2C_SENSOR_GetPressure();
                    f_temp = (uint8)(temp / 40.0f * (TEMP_40 - TEMP_ZERO)) + TEMP_ZERO;
                    f_hum = (uint8)((pressure - 950.0) / 50.0 * (HUM_50 - HUM_ZERO)) + HUM_ZERO;
                    // パケットはPACKET_PERIODごとに送信される
                    if (++tick >= PACKET_PERIOD) {
                        tick = 0;
                        state = ST_SEND;
                    } else {
                        state = ST_SHOW;
                    }
                    break;
                case ST_SEND:
                    // Major フィールドの設定
                    cyBle_discoveryData.advData[ID_OFFSET] = f_id;
                    
                    // Minor フィールドの設定
                    cyBle_discoveryData.advData[TEMP_OFFSET] = f_temp;
                    cyBle_discoveryData.advData[HUM_OFFSET] = f_hum;

                    // Advertisement パケットの送信を開始する
                    CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
                    
                    state = ST_STOP;
                    break;
                case ST_STOP:                
                    // Advertisement パケットの送信を止める
                    CyBle_GappStopAdvertisement();
                    state = ST_SHOW;
                    break;
                case ST_SHOW:
                    // 温度と気圧をLCDに表示する
                    sprintf(sbuf, "T:%7.2fdegC", temp);
                    I2C_LCD_Position(0, 0x80);  // locate to 0,0
                    I2C_LCD_PrintString(sbuf);
                    sprintf(sbuf, "P:%7.2fmBar", pressure);
                    I2C_LCD_Position(0, 0xA8);  // locate to 1,0
                    I2C_LCD_PrintString(sbuf);
                    state = ST_FORCE;
                    break;
                default:
                    state = ST_INIT;
            }
        }

        // BLE スタックへのイベントを処理する
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
