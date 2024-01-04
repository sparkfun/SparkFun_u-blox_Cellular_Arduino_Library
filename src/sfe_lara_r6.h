#ifndef SFE_LARA_R6_LIBRARY_H
#define SFE_LARA_R6_LIBRARY_H

#include "sfe_ublox_cellular.h"
#include "sfe_ublox_cellular_voice.h"

// Base LARA-R6 class
class SparkFun_ublox_LARA_R6 : public SparkFun_ublox_Cellular
{
};

class SparkFun_ublox_LARA_R6001 : public SparkFun_ublox_LARA_R6, public SparkFun_ublox_Cellular_Voice_Base<SparkFun_ublox_LARA_R6001>
{
};

class SparkFun_ublox_LARA_R6001D : public SparkFun_ublox_LARA_R6
{
};

class SparkFun_ublox_LARA_R6401 : public SparkFun_ublox_LARA_R6, public SparkFun_ublox_Cellular_Voice_Base<SparkFun_ublox_LARA_R6401>
{
};

class SparkFun_ublox_LARA_R6401D : public SparkFun_ublox_LARA_R6
{
};

class SparkFun_ublox_LARA_R6801_00B : public SparkFun_ublox_LARA_R6, public SparkFun_ublox_Cellular_Voice_Base<SparkFun_ublox_LARA_R6801_00B>
{
};

class SparkFun_ublox_LARA_R6801D : public SparkFun_ublox_LARA_R6
{
};

#endif