#pragma once

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#include "utils/macros.h"



#define NETWORK_MAX_WIFI_ATTEMPTS 20


bool network_setup();
void network_update();

#endif /* _NETWORK_H_ */

