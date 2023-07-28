# EBYTE组件

## 初始化

```c
void Ebyte_Init( ebyte_config_t ebyte_config, ebyte_status_t *ebyte_status )
{
    Ebyte_RF.Install( ebyte_config, ebyte_status );   //初始化SPI

    xSemaphoreTake( ebyte_status->xmutex, portMAX_DELAY );

    Ebyte_RF.Init( ebyte_status );//初始化ebyte

    xSemaphoreGive( ebyte_status->xmutex );
}
```



```c
/*!
 * init radio chip by commands with status struct
 *
 * @param ebyte_status     the ebyte status struct
 */
void Ebyte_E220x_Init( ebyte_status_t *ebyte_status )
{
    bool enable = false;
    uint8_t SymbNum = 0;

    Ebyte_E220x_Reset( ebyte_status );
    Ebyte_E220x_Wakeup( ebyte_status );

    Ebyte_E220x_SetStandby( ebyte_status, STDBY_RC );  //设置待机
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTB_TRIM, XTA_TRIM_VALUE); 

    if ( ebyte_status->rxen_io == -1 && ebyte_status->txen_io == -1) {
        Ebyte_E220x_SetDio2AsRfSwitchCtrl( ebyte_status, 1 );
    }

    ebyte_status->OperatingMode = MODE_STDBY_RC;

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    Ebyte_E220x_SetRegulatorMode( ebyte_status, USE_DCDC );

    Ebyte_E220x_SetBufferBaseAddress( ebyte_status, 0x00, 0x00 );

    Ebyte_E220x_SetTxPaParams( ebyte_status, 22, RADIO_RAMP_200_US );

    Ebyte_E220x_SetDioIrqParams(ebyte_status, IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                        IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                        IRQ_RADIO_NONE ,
                        IRQ_RADIO_NONE); 

    uint32_t fs = ( RF_FREQUENCY_START + ( RF_FREQUENCY_CH * RF_FREQUENCY_CH_STEP ) );
    Ebyte_E220x_SetRfFrequency( ebyte_status, fs );
    
    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        /***************************TX config************************************/
        Ebyte_E220x_SetModem( ebyte_status, PACKET_TYPE_LORA );

        /* LoRa 2.4KHz(SF11)  BW(500KHz) CR(4/5) */
        RadioLoRaBandwidths_t bw = (RadioLoRaBandwidths_t)(Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][0]);
        RadioLoRaSpreadingFactors_t sf = (RadioLoRaSpreadingFactors_t)(Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][1]);
        RadioLoRaCodingRates_t cr = ((RadioLoRaCodingRates_t)Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][2] );
        Ebyte_E220x_SetModulationLoraParams ( ebyte_status, sf,  bw, cr);

        Ebyte_E220x_SetLoraPacketParams( ebyte_status, 0xFF );
        /***************************RX config************************************/
        Ebyte_E220x_SetStopRxTimerOnPreambleDetect( ebyte_status, enable );
        Ebyte_E220x_SetLoRaSymbNumTimeout( ebyte_status, SymbNum );
    }          
    else {
        Ebyte_E220x_SetModem( ebyte_status, PACKET_TYPE_GFSK );
    
        uint32_t bw = Ebyte_E220x_GetFskBandwidthRegValue( RF_FSK_BANDWIDTH );
        Ebyte_E220x_SetModulationFskParams( ebyte_status, RF_FSK_FREQUENCY_DEVIATION , bw , MOD_SHAPING_G_BT_1, RF_FSK_DATA_RATE);
        
        Ebyte_E220x_SetFskPacketParams( ebyte_status, 0xFF );
        
        Ebyte_E220x_SetFskSyncWord( ebyte_status );
        
        Ebyte_E220x_SetWhiteningSeed( ebyte_status, 0x01FF );
    }

    Ebyte_E220x_SetRx( ebyte_status, 0 );
}
```





## 发送数据

```c
/*!
 * radio chip send data
 *
 * @param payload the data buffer waiting to send
 * @param size    send buffer size
 * @param timeout operate waiting time (if set to 0, device will stay in rx mode until reception occurs)
 */
uint16_t Ebyte_E220x_SendPayload( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size, uint32_t timeout )
{
    uint16_t irq_return;

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        Ebyte_E220x_SetLoraPacketParams( ebyte_status, size );
    }
    else
    {
        Ebyte_E220x_SetFskPacketParams( ebyte_status, size );
    }
    
    Ebyte_E220x_SetPayload( ebyte_status, payload, size );

    Ebyte_E220x_SetTx( ebyte_status, timeout );

    do
    {
        Ebyte_Port_DelayMs( 1 );
        Ebyte_E220x_GetIrqStatus(ebyte_status);
    }
    while( !ebyte_status->irqStatus );
    
    irq_return = ebyte_status->irqStatus;

    Ebyte_Port_TransmitCallback( ebyte_status->irqStatus );
    
    Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );

    return irq_return;
}
```

## 读取数据

```c
/* !
 * @brief  receive data with ebyte radio chip once
 *
 * @param  handle         the handler of ebyte
 * @param  payload        array of received data
 * @param  timeout_ms     the waiting time for receive data, if it times out, a timeout interrupt will be triggered
 *                        max num is 262000 (262s)
 *                        (if set to 0, radio chip will stay in RX status until data is received)
 * @return the data len has received
 */
int Ebyte_Receive( ebyte_handle_t handle, uint8_t *payload, uint32_t timeout_ms )
{
    ebyte_status_t *ebyte_status = (ebyte_status_t *)handle;
    if( timeout_ms > 262000 )
    {
        ets_printf("Set too large timeout\n");
        return -1;
    }
    uint32_t timeout = timeout_ms * 1000 / 15;
    uint16_t irq_return;

    xSemaphoreTake( ebyte_status->xmutex, portMAX_DELAY );
    Ebyte_RF.EnterReceiveMode( ebyte_status, timeout );  //Ebyte_E220x_SetRx
    do {
        irq_return = Ebyte_RF.StartPollTask(ebyte_status, payload);//Ebyte_E220x_IntOrPollTask
        vTaskDelay( 10 );
    }
    while( !irq_return );
    xSemaphoreGive( ebyte_status->xmutex );
    if((irq_return & 0x0002) ==  0x0002 ) {
        return ebyte_status->recv_len;
    }
    else{
        return -1;
    }
}
```



```c
void Ebyte_E220x_SetRx( ebyte_status_t *ebyte_status, uint32_t timeout )
{
    uint8_t buf[3];

    if ( ebyte_status->txen_io != -1 && ebyte_status->rxen_io != -1 ) {
        Ebyte_Port_IoControl( ebyte_status->rxen_io, 1 );
        Ebyte_Port_IoControl( ebyte_status->txen_io, 0 );
    }

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTB_TRIM, XTA_TRIM_VALUE); 

    Ebyte_E220x_SetDioIrqParams(ebyte_status, IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                          IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                          IRQ_RADIO_NONE ,
                          IRQ_RADIO_NONE);    
    
    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        Ebyte_E220x_SetLoraPacketParams(ebyte_status, 0xFF);
    }
    else{
        Ebyte_E220x_SetFskPacketParams(ebyte_status, 0xFF);
    }
    
    Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );

    buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( timeout & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RX, buf, 3 );
}
```





```c
/*!
 * Query the interrupt status of the radio chip, if rx irq occured, trigger rx handler
 *
 * @param ebyte_status     the ebyte status struct
 */
uint16_t Ebyte_E220x_IntOrPollTask( ebyte_status_t *ebyte_status, uint8_t *receive_data )
{
    Ebyte_E220x_GetIrqStatus(ebyte_status);

    uint16_t irq_return = ebyte_status->irqStatus;

    if( ebyte_status->irqStatus != 0 )
    {
        if( ( ebyte_status->irqStatus & IRQ_RX_DONE ) ==  IRQ_RX_DONE )
        {
            if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
            {
                Ebyte_E220x_GetLoraPacketStatus( ebyte_status );
            }          
            else {
                Ebyte_E220x_GetFskPacketStatus( ebyte_status );
            }        
            Ebyte_E220x_GetPayload( ebyte_status, RF_RX_BUFFER_SIZE );
            
            Ebyte_Port_ReceiveCallback( ebyte_status->irqStatus, ebyte_status->RecvPacketBuffer, receive_data, ebyte_status->recv_len );
            
            Ebyte_E220x_SetRx( ebyte_status, 0 );
        }
        else 
        {
            Ebyte_Port_ReceiveCallback( ebyte_status->irqStatus, ebyte_status->RecvPacketBuffer, receive_data, ebyte_status->recv_len );
        }

        Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );
    }

    return irq_return;
}
```

