#ifndef SFE_LARA_R6_LIBRARY_H
#define SFE_LARA_R6_LIBRARY_H

#include "sfe_ublox_cellular.h"
#include "sfe_ublox_cellular_voice.h"

// Base LARA-R6 class
class LARA_R6 : public UBX_CELL
{
};

class LARA_R6001 : public LARA_R6, public UBX_CELL_VOICE<LARA_R6001>
{
};

class LARA_R6001D : public LARA_R6
{
};

class LARA_R6401 : public LARA_R6, public UBX_CELL_VOICE<LARA_R6401>
{
};

class LARA_R6401D : public LARA_R6
{
};

class LARA_R6801_00B : public LARA_R6, public UBX_CELL_VOICE<LARA_R6801_00B>
{
};

class LARA_R6801D : public LARA_R6
{
};

#endif