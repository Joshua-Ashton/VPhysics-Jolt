
#include "cbase.h"

#include "vjolt_interface.h"
#include "vjolt_keyvalues_schema.h"

#include "vjolt_surfaceprops.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-------------------------------------------------------------------------------------------------

JoltPhysicsSurfaceProps JoltPhysicsSurfaceProps::s_PhysicsSurfaceProps;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( JoltPhysicsSurfaceProps, IPhysicsSurfaceProps, VPHYSICS_SURFACEPROPS_INTERFACE_VERSION, JoltPhysicsSurfaceProps::GetInstance() );

//-------------------------------------------------------------------------------------------------

static const JoltKVSchemaProp_t kSurfacePropDescs[] =
{
	// Base Property
	{ "Base",								KVSCHEMA_DESC( JoltSurfaceProp, data ),											FillBaseProp },

	// Physics Properties
	{ "Friction",							KVSCHEMA_DESC( JoltSurfaceProp, data.physics.friction ),						FillFloatProp },
	{ "Elasticity",							KVSCHEMA_DESC( JoltSurfaceProp, data.physics.elasticity ),						FillFloatProp },
	{ "Density",							KVSCHEMA_DESC( JoltSurfaceProp, data.physics.density ),							FillFloatProp },
	{ "Thickness",							KVSCHEMA_DESC( JoltSurfaceProp, data.physics.thickness ),						FillFloatProp },
	{ "Dampening",							KVSCHEMA_DESC( JoltSurfaceProp, data.physics.dampening ),						FillFloatProp },

	// Audio Properties
	{ "AudioReflectivity",					KVSCHEMA_DESC( JoltSurfaceProp, data.audio.reflectivity ),						FillFloatProp },
	{ "AudioHardnessFactor",				KVSCHEMA_DESC( JoltSurfaceProp, data.audio.hardnessFactor ),					FillFloatProp },
	{ "AudioRoughnessFactor",				KVSCHEMA_DESC( JoltSurfaceProp, data.audio.roughnessFactor ),					FillFloatProp },

	{ "ScrapeRoughThreshold",				KVSCHEMA_DESC( JoltSurfaceProp, data.audio.roughThreshold ),					FillFloatProp },
	{ "ImpactHardThreshold",				KVSCHEMA_DESC( JoltSurfaceProp, data.audio.hardThreshold ),						FillFloatProp },
	{ "AudioHardMinVelocity",				KVSCHEMA_DESC( JoltSurfaceProp, data.audio.hardVelocityThreshold ),				FillFloatProp },

#ifdef GAME_CSGO_OR_NEWER
	{ "HighPitchOcclusion",					KVSCHEMA_DESC( JoltSurfaceProp, data.audio.highPitchOcclusion ),				FillFloatProp },
	{ "MidPitchOcclusion",					KVSCHEMA_DESC( JoltSurfaceProp, data.audio.midPitchOcclusion ),					FillFloatProp },
	{ "LowPitchOcclusion",					KVSCHEMA_DESC( JoltSurfaceProp, data.audio.lowPitchOcclusion ),					FillFloatProp },
#endif

	// Sound Properties
#ifdef GAME_ASW_OR_NEWER
	{ "StepLeft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.walkStepLeft ),						FillSoundProp },
	{ "StepLeft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.runStepLeft ),						FillSoundProp },
	{ "StepRight",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.walkStepRight ),					FillSoundProp },
	{ "StepRight",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.runStepRight ),						FillSoundProp },

	{ "WalkLeft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.walkStepLeft ),						FillSoundProp },
	{ "WalkRight",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.walkStepRight ),					FillSoundProp },

	{ "RunLeft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.runStepLeft ),						FillSoundProp },
	{ "RunRight",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.runStepRight ),						FillSoundProp },
#else
	{ "StepLeft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.stepleft ),							FillSoundProp },
	{ "StepRight",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.stepright ),						FillSoundProp },
#endif

	{ "ImpactSoft",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.impactSoft ),						FillSoundProp },
	{ "ImpactHard",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.impactHard ),						FillSoundProp },

	{ "ScrapeSmooth",						KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.scrapeSmooth ),						FillSoundProp },
	{ "ScrapeRough",						KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.scrapeRough ),						FillSoundProp },

	{ "BulletImpact",						KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.bulletImpact ),						FillSoundProp },
	{ "Rolling",							KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.rolling ),							FillSoundProp },

	{ "Break",								KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.breakSound ),						FillSoundProp },
	{ "Strain",								KVSCHEMA_DESC( JoltSurfaceProp, data.sounds.strainSound ),						FillSoundProp },

	// Game Properties
	{ "MaxSpeedFactor",						KVSCHEMA_DESC( JoltSurfaceProp, data.game.maxSpeedFactor ),						FillFloatProp },
	{ "JumpFactor",							KVSCHEMA_DESC( JoltSurfaceProp, data.game.jumpFactor ),							FillFloatProp },
#ifdef GAME_CSGO_OR_NEWER
	{ "PenetrationModifier",				KVSCHEMA_DESC( JoltSurfaceProp, data.game.penetrationModifier ),				FillFloatProp },
	{ "DamageModifier",						KVSCHEMA_DESC( JoltSurfaceProp, data.game.damageModifier ),						FillFloatProp },
#endif
	{ "GameMaterial",						KVSCHEMA_DESC( JoltSurfaceProp, data.game.material ),							FillGameMaterialProp },
	{ "Climbable",							KVSCHEMA_DESC( JoltSurfaceProp, data.game.climbable ),							FillUnsignedCharProp },
#ifdef GAME_CSGO_OR_NEWER
	{ "HideTargetID",						KVSCHEMA_DESC( JoltSurfaceProp, data.game.hidetargetid ),						FillBoolProp },
	{ "DamageLossPercentPerPenetration",	KVSCHEMA_DESC( JoltSurfaceProp, data.game.damageLossPercentPerPenetration ),	FillFloatProp },
#endif
};

//-------------------------------------------------------------------------------------------------

JoltPhysicsSurfaceProps::JoltPhysicsSurfaceProps()
{
	JoltSurfaceProp prop = {};
	prop.data.physics.friction		= 0.8f;
	prop.data.physics.elasticity	= 0.25f;
	prop.data.physics.density		= 2000.0f;
	prop.data.physics.thickness		= 0.0f;
	prop.data.physics.dampening		= 0.0f;
	m_SurfaceProps[ "default" ] = prop;

	// Game code uses 0 as invalid index, expects empty string
	m_SoundStrings.AddString("");

	m_ShadowMaterialIndex = 0;
	m_SetupShadowMaterial = false;

	// Following IVP's behavior - they initially map to themselves
	for ( int i = 0; i < MaxSurfaceMaterials; ++i )
		m_MaterialPropMap[ i ] = i;
}

//-------------------------------------------------------------------------------------------------

int JoltPhysicsSurfaceProps::ParseSurfaceData( const char *pFilename, const char *pTextfile )
{
	KeyValues::AutoDelete kv( SurfacePropsToKeyValues( pTextfile ) );

	for ( KeyValues* pSurface = kv->GetFirstSubKey(); pSurface != nullptr; pSurface = pSurface->GetNextKey() )
	{
		const char *pSurfaceName = pSurface->GetName();

		JoltSurfaceProp values = {};
		// Try to find it if we already have a material with this name,
		// so we can update the values on it, otherwise take the base material.
		UtlSymId_t id = m_SurfaceProps.Find( pSurfaceName );
		if ( id != m_SurfaceProps.InvalidIndex() )
			values = m_SurfaceProps[ id ];
		else
			values = m_SurfaceProps[ BaseMaterialIdx ];

		ParseJoltKVSchema( pSurface, kSurfacePropDescs, uint( std::size( kSurfacePropDescs ) ), &values );

		// If we don't have this already, add it,
		// otherwise update the values.
		if ( id == m_SurfaceProps.InvalidIndex() )
			m_SurfaceProps[ pSurfaceName ] = values;
		else
			m_SurfaceProps[ id ] = values;
	}

	if ( !m_SetupShadowMaterial )
	{
		JoltSurfaceProp values = {};
		values.data = m_SurfaceProps[ BaseMaterialIdx ].data;
		// RaphaelIT7: IVP values - probably need adjusting / should be checked again
		values.data.physics.friction = 0.8f;
		values.data.physics.elasticity = 1e-3f;

		m_ShadowMaterialIndex = m_SurfaceProps.GetNumStrings();
		m_SurfaceProps[ GetReservedMaterialName( ShadowMaterialIndex ) ] = values;

		m_SetupShadowMaterial = true;
	}

	return m_SurfaceProps.GetNumStrings();
}

int JoltPhysicsSurfaceProps::SurfacePropCount( void ) const
{
	return int ( m_SurfaceProps.GetNumStrings() );
}

//-------------------------------------------------------------------------------------------------

int JoltPhysicsSurfaceProps::GetSurfaceIndex( const char *pSurfacePropName ) const
{
	if ( pSurfacePropName[0] == '$' )
	{
		if ( V_stricmp( pSurfacePropName, "$MATERIAL_INDEX_SHADOW" ) )
			return ShadowMaterialIndex;
	}

	UtlSymId_t nIndex = m_SurfaceProps.Find( pSurfacePropName );
	if ( nIndex != m_SurfaceProps.InvalidIndex() )
		return int( nIndex );

	return -1;
}

void JoltPhysicsSurfaceProps::GetPhysicsProperties( int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity ) const
{
	surfaceDataIndex = RemapMaterialIndexForReserved( surfaceDataIndex );
	const UtlSymId_t id = surfaceDataIndex >= 0 && surfaceDataIndex < int( m_SurfaceProps.GetNumStrings() )
		? UtlSymId_t( surfaceDataIndex )
		: BaseMaterialIdx;

	const JoltSurfaceProp& prop = m_SurfaceProps[ id ];
	if ( density )		*density	= prop.data.physics.density;
	if ( thickness )	*thickness	= prop.data.physics.thickness;
	if ( friction )		*friction	= prop.data.physics.friction;
	if ( elasticity )	*elasticity	= prop.data.physics.elasticity;
}

//-------------------------------------------------------------------------------------------------

surfacedata_t *JoltPhysicsSurfaceProps::GetSurfaceData( int surfaceDataIndex )
{
	surfaceDataIndex = RemapMaterialIndexForReserved( surfaceDataIndex );
	const UtlSymId_t id = surfaceDataIndex >= 0 && surfaceDataIndex < int( m_SurfaceProps.GetNumStrings() )
		? UtlSymId_t( surfaceDataIndex )
		: BaseMaterialIdx;

	JoltSurfaceProp& prop = m_SurfaceProps[ id ];
	return &prop.data;
}

const char *JoltPhysicsSurfaceProps::GetString( unsigned short stringTableIndex ) const
{
	return m_SoundStrings.String( stringTableIndex );
}

//-------------------------------------------------------------------------------------------------

const char *JoltPhysicsSurfaceProps::GetPropName( int surfaceDataIndex ) const
{
	surfaceDataIndex = RemapMaterialIndexForReserved( surfaceDataIndex );
	if ( surfaceDataIndex < 0 || surfaceDataIndex >= int ( m_SurfaceProps.GetNumStrings() ) )
		return nullptr;

	return m_SurfaceProps.String( surfaceDataIndex );
}

//-------------------------------------------------------------------------------------------------

void JoltPhysicsSurfaceProps::SetWorldMaterialIndexTable( int *pMapArray, int mapSize )
{
	if ( mapSize > MaxSurfaceMaterials )
		mapSize = MaxSurfaceMaterials;

	for ( int i = 0; i < mapSize; ++i )
		m_MaterialPropMap[ i ] = (unsigned short)pMapArray[ i ];
}

//-------------------------------------------------------------------------------------------------

void JoltPhysicsSurfaceProps::GetPhysicsParameters( int surfaceDataIndex, surfacephysicsparams_t *pParamsOut ) const
{
	if ( !pParamsOut )
		return;

	const UtlSymId_t id = surfaceDataIndex >= 0 && surfaceDataIndex < int( m_SurfaceProps.GetNumStrings() )
		? UtlSymId_t( surfaceDataIndex )
		: BaseMaterialIdx;

	const JoltSurfaceProp& prop = m_SurfaceProps[ id ];
	*pParamsOut = prop.data.physics;
}

//-------------------------------------------------------------------------------------------------

ISaveRestoreOps *JoltPhysicsSurfaceProps::GetMaterialIndexDataOps() const
{
	return &JoltPhysicsMaterialIndexSaveOps::GetInstance();
}

//-------------------------------------------------------------------------------------------------

unsigned short JoltPhysicsSurfaceProps::RegisterSound( const char *pName )
{
	return m_SoundStrings.AddString( pName );
}

//-------------------------------------------------------------------------------------------------

KeyValues *JoltPhysicsSurfaceProps::SurfacePropsToKeyValues( const char *pszBuffer )
{
	return HeaderlessKVBufferToKeyValues( pszBuffer, "PhysProps" );
}

//-------------------------------------------------------------------------------------------------

void *JoltPhysicsSurfaceProps::GetIVPMaterial( int nIndex )
{
	nIndex = RemapMaterialIndexForReserved( nIndex );

	if ( nIndex < 0 || nIndex > ( m_SurfaceProps.GetNumStrings() - 1 ) )
		return nullptr;

	// return &m_SurfaceProps[ nIndex ];
	return nullptr; // RaphaelIT7: Current issue - the struct probably differs from IVP's class which would cause all sorts of issues :/
}

int JoltPhysicsSurfaceProps::GetIVPMaterialIndex( const void *pMaterial ) const
{
	// int nIndex = ( (char*)pMaterial - (char*)&m_SurfaceProps[0] ) / sizeof( JoltSurfaceProp ); - RaphaelIT7: Verify later again...
	Log_Stub( LOG_VJolt );
	return -1; // IVP's default
}

void *JoltPhysicsSurfaceProps::GetIVPManager( void )
{
	// RaphaelIT7: Should be unused - was used by vphysics internally from what I could find
	Log_Stub( LOG_VJolt );
	return nullptr;
}

int JoltPhysicsSurfaceProps::RemapIVPMaterialIndex( int nIndex ) const
{
	if ( nIndex >= MaxSurfaceMaterials || nIndex < 0 )
		return nIndex;

	return m_MaterialPropMap[ nIndex ];
}

const char *JoltPhysicsSurfaceProps::GetReservedMaterialName( int nMaterialIndex ) const
{
	if ( nMaterialIndex == ShadowMaterialIndex )
		return "$MATERIAL_INDEX_SHADOW";

	return NULL;
}

//-------------------------------------------------------------------------------------------------

JoltPhysicsMaterialIndexSaveOps JoltPhysicsMaterialIndexSaveOps::s_Instance;

void JoltPhysicsMaterialIndexSaveOps::Save( const SaveRestoreFieldInfo_t &fieldInfo, ISave *pSave )
{
	int *pMaterialIdx = reinterpret_cast<int*>( fieldInfo.pField );

	const char *pMaterialName = JoltPhysicsSurfaceProps::GetInstance().GetPropName( *pMaterialIdx );
	if ( !pMaterialName )
		pMaterialName = JoltPhysicsSurfaceProps::GetInstance().GetPropName( 0 );

	int nMaterialNameLength = V_strlen( pMaterialName ) + 1;
	pSave->WriteInt( &nMaterialNameLength );
	pSave->WriteString( pMaterialName );
}

void JoltPhysicsMaterialIndexSaveOps::Restore( const SaveRestoreFieldInfo_t &fieldInfo, IRestore *pRestore )
{
	int nMaterialNameLength = pRestore->ReadInt();
	char szMaterialName[ 2048 ];
	pRestore->ReadString( szMaterialName, sizeof( szMaterialName ), nMaterialNameLength );

	int *pMaterialIdx = reinterpret_cast<int*>( fieldInfo.pField );
	*pMaterialIdx = Max( JoltPhysicsSurfaceProps::GetInstance().GetSurfaceIndex( szMaterialName ), 0 );
}

bool JoltPhysicsMaterialIndexSaveOps::IsEmpty( const SaveRestoreFieldInfo_t &fieldInfo )
{
	int *pMaterialIdx = reinterpret_cast<int*>( fieldInfo.pField );
	return !*pMaterialIdx;
}

void JoltPhysicsMaterialIndexSaveOps::MakeEmpty( const SaveRestoreFieldInfo_t &fieldInfo )
{
	int* pMaterialIdx = reinterpret_cast<int*>( fieldInfo.pField );
	*pMaterialIdx = 0;
}
