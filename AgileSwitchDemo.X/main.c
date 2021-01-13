/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include <string.h>
#include <stdio.h>
#include "mcc_generated_files/system.h"
//#include "mcc_generated_files/examples/uart_example.h"
#include "mcc_generated_files/drivers/uart.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/pin_manager.h"

/*
                         Main application
 */

uint8_t tempL, tempM, tempH = 0;
uint8_t temp4D, temp3D, temp2D, temp1D = 0;
uint16_t u16Duty, u16Period = 0;
uint32_t u32Duty, u32Period = 0;
uint16_t u16Dummy = 0;
uint32_t u32Temperature = 0;
int16_t i16Voltage = 0;
uint16_t u16Cnt_500ms = 0;

void FaultDetect(void);
void LedIndication(void);
void SwitchDetect(void);
void PWM_DSET_TEST(bool flag1, bool flag2, bool flag3);

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();   
    DELAY_milliseconds(500);
    printf("=========================================== \n");
    printf("AgileSwitch Demonstration with dsPIC33CK \n");
    printf("=========================================== \n");
    
    while (1)
    {
        // Add your application code
        if(bFlag100ms == true)
        {
            bFlag100ms = false;
            
            FaultDetect();
            LedIndication();
            SwitchDetect();
//            PWM_DSET_TEST(bHiSideDSET, bLoSideDSET, bReset);
            
            // get the temperature data  
            u16Duty = u16TOn;
            u16Period = u16TOn + u16TOff;
            
            if(u16Period < 1)
            {
                u16Period = 1;
            }

            if(bCap1NoneSense == true)
            {
                CCP1STATLbits.ICOV = 0;
                if(PORTDbits.RD2 == 1)
                {
                    u16Duty = 100;
                }
                else
                {
                    u16Duty = 0;
                }
            }
            
            u32Temperature = __builtin_divud(__builtin_muluu(750, u16Duty), u16Period);
            u32Temperature = (u32Temperature + 25) >> 2;
            
            // get the voltage data
            u16Duty = u16T2On;
            u16Period = u16T2On + u16T2Off;
            
            if(u16Period < 1)
            {
                u16Period = 1;
            }

            if(bCap2NoneSense == true)
            {
                CCP2STATLbits.ICOV = 0;
                if(PORTDbits.RD5 == 1)
                {
                    u16Duty = 100;
                }
                else
                {
                    u16Duty = 0;
                }
            }
            
            i16Voltage = __builtin_divud(__builtin_muluu(1250, u16Duty), u16Period);
            i16Voltage = i16Voltage - 125;
            
            if(i16Voltage < 0)
                i16Voltage = 0;
            
            if(u16Cnt_500ms < 5)
            {
                u16Cnt_500ms++;
            }
            else
            {
                u16Cnt_500ms = 0;

                // data & status reporting
                temp3D = u32Temperature / 100;
                temp2D = (u32Temperature % 100) / 10;
                temp1D = u32Temperature % 10;
                printf("Temperature : %d%d%d degree C \n", temp3D, temp2D, temp1D);

                temp4D = i16Voltage / 1000;
                temp3D = (i16Voltage % 1000) / 100;
                temp2D = (i16Voltage % 100) / 10;
                temp1D = i16Voltage % 10;
                printf("Voltage : %d%d%d%d V \n",temp4D, temp3D, temp2D, temp1D);

                if(bHiFault == true)
                {
                    printf("====== HI-FAULT !!! ====== \n");
                }
                if(bLoFault == true)
                {
                    printf("====== LO-FAULT !!! ====== \n");
                }
                if(bAllFault == true)
                {
                    printf("====== ALL-FAULT !!! ====== \n");
                }
                
            }
                    
            
        }
    }
    return 1; 
}

void FaultDetect(void)
{
    if( HI_FAULT_GetValue() == 0 )
    {
        bHiFault = true;
    }
    else
    {
        bHiFault = false;
    }
    
    if( LO_FAULT_GetValue() == 0 )
    {
        bLoFault = true;
    }
    else
    {
        bLoFault = false;
    }

    if( ALL_FAULT_GetValue() == 0 )
    {
        bAllFault = true;
    }
    else
    {
        bAllFault = false;
    }    
}

void LedIndication(void)
{
    if(bHiFault == true)
    {
        LED1Control(LED_BLINK);
//        printf("====== HI-FAULT !!! ====== \n");
    }
    else
    {
        LED1Control(LED_OFF);
    }

    if(bLoFault == true)
    {
        LED2Control(LED_BLINK);
//        printf("====== LO-FAULT !!! ====== \n");
    }
    else
    {
        LED2Control(LED_OFF);
    }

    if(bAllFault == true)
    {
        LED3Control(LED_BLINK);
//        printf("====== ALL-FAULT !!! ====== \n");
    }
    else
    {
        LED3Control(LED_OFF);
    }    
}

void SwitchDetect(void)
{
    static uint8_t sw1_cnt, sw2_cnt, sw3_cnt = 0;
    static bool bSW1, bSW2, bSW3 = false;
    
    if(SW1_GetValue() == 0)
    {
        if(sw1_cnt >= 3)
        {
            bSW1 = true;
        }
        else
        {
           sw1_cnt++; 
        }
    }
    else
    {
        if(bSW1 == true)
        {
            bSW1 = false;
            bHiSideDSET = true;
            bLoSideDSET = false;
            bReset = false;
        }
        sw1_cnt = 0;
    }
    
    if(SW2_GetValue() == 0)
    {
        if(sw2_cnt >= 3)
        {
            bSW2 = true;
        }
        else
        {
           sw2_cnt++; 
        }
    }
    else
    {
        if(bSW2 == true)
        {
            bSW2 = false;
            bLoSideDSET = true;
            bHiSideDSET = false;
            bReset = false;
        }
        sw2_cnt = 0;
    }

    if(SW3_GetValue() == 0)
    {
        if(sw3_cnt >= 3)
        {
            bSW3 = true;
            RESET_OUTPUT_SetHigh();
        }
        else
        {
           sw3_cnt++; 
        }
        
    }
    else
    {
        if(bSW3 == true)
        {
            bSW3 = false;
            bLoSideDSET = false;
            bHiSideDSET = false;
            bReset = true;
        }
        sw3_cnt = 0;
        RESET_OUTPUT_SetLow();
    }     
}

void PWM_DSET_TEST(bool flag1, bool flag2, bool flag3)
{
//    static uint8_t cnt1, cnt2, cnt3 = 0;
    
//    if(flag1 == true)
//    {
//        if(cnt1 > 10)
//        {
//            //PG1IOCONLbits.OVRENH = 0;
//            HI_DSET_SetHigh();
//        }
//        else
//        {
//            cnt1++;
//            HI_DSET_SetLow();
//            //PG1IOCONLbits.OVRENH = 1;
//            //PG1IOCONLbits.OVRENL = 1;
//        }
//    }
//    else
//    {
//        cnt1 = 0;
//        HI_DSET_SetHigh();
//    }
    
//    if(flag2 == true)
//    {
//        if(cnt2 > 10)
//        {
//            //PG1IOCONLbits.OVRENL = 0;
//            LO_DSET_SetHigh();
//        }
//        else
//        {
//            cnt2++;
//            LO_DSET_SetLow();
//            //PG1IOCONLbits.OVRENH = 1;
//            //PG1IOCONLbits.OVRENL = 1;
//        }
//    }
//    else
//    {
//        cnt2 = 0;
//        LO_DSET_SetHigh();
//    }

//    if(flag3 == true)
//    {
//        if(cnt3 > 3)
//        {
//            //PG1IOCONLbits.OVRENH = 0;
//            //PG1IOCONLbits.OVRENL = 0;
////            HI_DSET_SetHigh();
////            LO_DSET_SetHigh();
////            RESET_OUTPUT_SetHigh();             
//        }
//        else
//        {
//            cnt3++;
////            HI_DSET_SetLow();
////            LO_DSET_SetLow();
////            RESET_OUTPUT_SetLow();            
//        }        
//    }
//    else
//    {
//        cnt3 = 0;
//    }
}

/**
 End of File
*/

