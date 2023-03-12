
void show_encoder_to_oled()
{
    // chargeMode();
	bsp_init();

    OLED12864_Init();

    while(1)
    {
        OLED12864_Clear();
        OLED12864_Show_Num( 0 , 0 , bsp_encoder_get_value(0) , 1 );
        OLED12864_Show_Num( 1 , 0 , bsp_encoder_get_value(1) , 1 );
        OLED12864_Show_Num( 2 , 0 , bsp_encoder_get_value(2) , 1 );
        OLED12864_Show_Num( 3 , 0 , bsp_encoder_get_value(3) , 1 );
        OLED12864_Show_Num( 5 , 0 , HAL_GetTick()/1000 , 1 );
        HAL_Delay( 20 );
    }
}
