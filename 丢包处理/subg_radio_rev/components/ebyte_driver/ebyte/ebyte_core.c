#include "ebyte_core.h"

const Ebyte_RF_t Ebyte_RF =
{
  Ebyte_E220x_Install,
  Ebyte_E220x_Unstall,
  Ebyte_E220x_Init,
  Ebyte_E220x_SendPayload,
  Ebyte_E220x_SetSleep,
  Ebyte_E220x_SetRx,  
  Ebyte_E220x_IntOrPollTask,
};






