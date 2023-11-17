#ifndef SFE_SARA_R5_LIBRARY_H
#define SFE_SARA_R5_LIBRARY_H

#include "sfe_ublox_cellular.h"

// Base SARA-R5 class
class SARA_R5: public UBLOX_AT
{

};

class SARA_R500S: public SARA_R5
{

};

class SARA_R500S_01B: public SARA_R5
{

};

class SARA_R500S_61B: public SARA_R5
{

};

class SARA_R510M8S_61B: public SARA_R5
{

};

class SARA_R510S: public SARA_R5
{

};

#endif