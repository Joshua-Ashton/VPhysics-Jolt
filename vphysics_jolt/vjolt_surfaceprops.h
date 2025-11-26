
#pragma once

// RaphaelIT7: IVP reminants :/
static constexpr int ShadowMaterialIndex = 0xF000;
static constexpr int MaxSurfaceMaterials = 128;

struct JoltSurfaceProp
{
	surfacedata_t data;
};

class JoltPhysicsMaterialIndexSaveOps : public CDefSaveRestoreOps
{
public:
	void Save( const SaveRestoreFieldInfo_t &fieldInfo, ISave *pSave ) override;
	void Restore( const SaveRestoreFieldInfo_t& fieldInfo, IRestore* pRestore ) override;

	bool IsEmpty( const SaveRestoreFieldInfo_t &fieldInfo ) override;
	void MakeEmpty( const SaveRestoreFieldInfo_t &fieldInfo ) override;

	static JoltPhysicsMaterialIndexSaveOps& GetInstance() { return s_Instance; }

private:
	static JoltPhysicsMaterialIndexSaveOps s_Instance;
};

class JoltPhysicsSurfaceProps final : public IPhysicsSurfaceProps
{
public:
	JoltPhysicsSurfaceProps();

	int					ParseSurfaceData( const char *pFilename, const char *pTextfile ) override;
	int					SurfacePropCount( void ) const override;

	int					GetSurfaceIndex( const char *pSurfacePropName ) const override;
	void				GetPhysicsProperties( int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity ) const override;

	surfacedata_t		*GetSurfaceData( int surfaceDataIndex ) override;
	const char			*GetString( unsigned short stringTableIndex ) const override;

	const char			*GetPropName( int surfaceDataIndex ) const override;

	void				SetWorldMaterialIndexTable( int *pMapArray, int mapSize ) override;

	void				GetPhysicsParameters( int surfaceDataIndex, surfacephysicsparams_t *pParamsOut ) const override;

	ISaveRestoreOps		*GetMaterialIndexDataOps() const override_portal2;

	// GMod-specific internal gubbins that was exposed in the public interface.
	void				*GetIVPMaterial( int nIndex ) override_gmod;
	int					GetIVPMaterialIndex( const void *pMaterial ) const override_gmod;
	void				*GetIVPManager( void ) override_gmod;
	int					RemapIVPMaterialIndex( int nIndex ) const override_gmod;
	const char 			*GetReservedMaterialName( int nMaterialIndex ) const override_gmod;

public:
	static JoltPhysicsSurfaceProps& GetInstance() { return s_PhysicsSurfaceProps; }

	unsigned short		RegisterSound( const char *pName );

	inline int RemapMaterialIndexForReserved( int nIndex ) const
	{
		if ( nIndex >= MaxSurfaceMaterials )
			return nIndex == ShadowMaterialIndex ? m_ShadowMaterialIndex : 0;

		return nIndex;
	};

	inline int GetShadowMaterialIndex() { return m_ShadowMaterialIndex; }

private:
	static JoltPhysicsSurfaceProps s_PhysicsSurfaceProps;

	CUtlStringMap< JoltSurfaceProp >	m_SurfaceProps;
	CUtlSymbolTable						m_SoundStrings;

	int									m_ShadowMaterialIndex = 0;
	bool								m_SetupShadowMaterial = false;
	unsigned short						m_MaterialPropMap[MaxSurfaceMaterials];
	
	static constexpr UtlSymId_t BaseMaterialIdx = UtlSymId_t( 0 );

	KeyValues *SurfacePropsToKeyValues( const char *pszBuffer );
};
