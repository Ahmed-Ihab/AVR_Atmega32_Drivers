#include "ExtInter_conf.h"



// (INT0),(INT0_LowLevel,INT0_LogicalChange, INT0_FallingEdge , INT0_RisingEdge), (Active, In_Active)
// (INT1),(INT1_LowLevel,INT1_LogicalChange, INT1_FallingEdge , INT1_RisingEdge), (Active, In_Active)
// (INT2),(INT2_FallingEdge,INT2_RisingEdge), (Active, In_Active)



Interrupt_Conf_t Interrupt_Array[Num_Of_Interrupt] =
{
{INT0, INT2_FallingEdge, In_Active} ,
{INT1, INT2_FallingEdge ,In_Active} ,
{INT2, INT2_RisingEdge, Active}
};
