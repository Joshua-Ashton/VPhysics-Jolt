
#pragma once

// This file provides compatibility with stuff used from the Alien Swarm SDK and above
// Source Engine branches for SDK 2013.

#include "Color.h"
#include "branch_overrides.h"

#if !defined( GAME_GMOD_64X )
enum LoggingSeverity_t
{
    LS_MESSAGE = 0,
    LS_WARNING = 1,
    LS_ASSERT = 2,
    LS_ERROR = 3,
    LS_HIGHEST_SEVERITY = 4,
};

static const int MAX_LOGGING_MESSAGE_LENGTH = 2048;
#else
#undef DECLARE_LOGGING_CHANNEL
#undef DEFINE_LOGGING_CHANNEL_NO_TAGS
#undef Log_Msg
#undef Log_Warning
#undef Log_Error
#undef InternalMsg
#endif

struct LoggingChannelInfo_t
{
	const char *pszName;
	int nFlags;
	LoggingSeverity_t nSeverity;
	Color color = Color(255, 255, 255);
};

#define DECLARE_LOGGING_CHANNEL( Channel ) extern LoggingChannelInfo_t g_LoggingInfo##Channel;

#define DEFINE_LOGGING_CHANNEL_NO_TAGS( Channel, ChannelName, ...  ) LoggingChannelInfo_t g_LoggingInfo##Channel = { ChannelName, __VA_ARGS__ };

// TODO
#define DevAssert( ... )
#define DevAssertMsg( ... )
#define AssertMsg_Internal( ... )
#define InternalMsg( Channel, Fmt, ... ) ConColorMsg( g_LoggingInfo##Channel.color, "[%s]" Fmt, g_LoggingInfo##Channel.pszName, ##__VA_ARGS__ )
#define Log_Msg( Channel, Fmt, ... ) InternalMsg( Channel, Fmt, ##__VA_ARGS__ )
#define Log_Warning( Channel, Fmt, ... ) InternalMsg( Channel, Fmt, ##__VA_ARGS__ )
#define Log_Error( Channel, Fmt, ... ) InternalMsg( Channel, Fmt, ##__VA_ARGS__ )

// Unused, just makes stuff cleaner to not have ifdef spam.
#if !defined( GAME_GMOD_64X )
enum collisionhints
{
    COLLISION_HINT_DEBRIS = 0x0001,
    COLLISION_HINT_STATICSOLID = 0x0002,
};
#endif

class IPhysicsCollisionSet;
class IPhysics;

struct ragdollcollisionrules_t
{
    void Defaults( IPhysics *pPhysics, IPhysicsCollisionSet *pSetIn )
    {
        pCollisionSet = pSetIn;
        bSelfCollisions = true;
    }
    int       bSelfCollisions;
    IPhysicsCollisionSet *pCollisionSet;
};

struct ragdollanimatedfriction_t
{
    float minFriction;
    float maxFriction;
    float timeIn;
    float timeOut;
    float timeHold;
};

enum PlayerContactState_t
{
    PLAYER_CONTACT_PHYSICS = 1,
    PLAYER_CONTACT_GAMEOBJECT = 2,
};
