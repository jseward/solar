//requires wsock32.lib, Ws2_32.lib

#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4146)
#pragma warning(disable:4702)
#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4701)

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "solar_dependencies/enet/callbacks.c"
#include "solar_dependencies/enet/compress.c"
#include "solar_dependencies/enet/host.c"
#include "solar_dependencies/enet/list.c"
#include "solar_dependencies/enet/packet.c"
#include "solar_dependencies/enet/peer.c"
#include "solar_dependencies/enet/protocol.c"
#include "solar_dependencies/enet/unix.c"
#include "solar_dependencies/enet/win32.c"

#pragma warning(pop)
