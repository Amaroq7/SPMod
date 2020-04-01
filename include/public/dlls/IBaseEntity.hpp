/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
/*
CBaseEntity
    CBaseDelay
        CBaseToggle
            CBaseItem
            CBaseMonster
                CBaseCycler
                CBasePlayer
                CBaseGroup
*/

/*
class CBaseEntity
{
public:
    // Constructor.  Set engine to use C/C++ callback functions
    // pointers to engine data
    entvars_t *pev;		// Don't need to save/restore this pointer, the engine resets it

    // path corners
    CBaseEntity			*m_pGoalEnt;// path corner we are heading towards
    CBaseEntity			*m_pLink;// used for temporary link-list operations.

    // initialization functions
    virtual void	Spawn( void ) { return; }
    virtual void	Precache( void ) { return; }
    virtual void	KeyValue( KeyValueData* pkvd) { pkvd->fHandled = FALSE; }
    virtual int		Save( CSave &save );
    virtual int		Restore( CRestore &restore );
    virtual int		ObjectCaps( void ) { return FCAP_ACROSS_TRANSITION; }
    virtual void	Activate( void ) {}

    // Setup the object->object collision box (pev->mins / pev->maxs is the object->world collision box)
    virtual void	SetObjectCollisionBox( void );

// Classify - returns the type of group (i.e, "houndeye", or "human military" so that monsters with different classnames
// still realize that they are teammates. (overridden for monsters that form groups)
    virtual int Classify ( void ) { return CLASS_NONE; };
    virtual void DeathNotice ( entvars_t *pevChild ) {}// monster maker children use this to tell the monster maker that
they have died.


    static	TYPEDESCRIPTION m_SaveData[];

    virtual void	TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
bitsDamageType); virtual int		TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int
bitsDamageType ); virtual int		TakeHealth( float flHealth, int bitsDamageType ); virtual void	Killed( entvars_t
*pevAttacker, int iGib ); virtual int		BloodColor( void ) { return DONT_BLEED; } virtual void	TraceBleed( float
flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType ); virtual BOOL    IsTriggered( CBaseEntity *pActivator )
{return TRUE;} virtual CBaseMonster *MyMonsterPointer( void ) { return NULL;} virtual CSquadMonster
*MySquadMonsterPointer( void ) { return NULL;} virtual	int		GetToggleState( void ) { return TS_AT_TOP; } virtual
void	AddPoints( int score, BOOL bAllowNegativeScore ) {} virtual void	AddPointsToTeam( int score, BOOL
bAllowNegativeScore ) {} virtual BOOL	AddPlayerItem( CBasePlayerItem *pItem ) { return 0; } virtual BOOL
RemovePlayerItem( CBasePlayerItem *pItem ) { return 0; } virtual int 	GiveAmmo( int iAmount, char *szName, int iMax )
{ return -1; }; virtual float	GetDelay( void ) { return 0; }
    virtual int		IsMoving( void ) { return pev->velocity != g_vecZero; }
    virtual void	OverrideReset( void ) {}
    virtual int		DamageDecal( int bitsDamageType );
    // This is ONLY used by the node graph to test movement through a door
    virtual void	SetToggleState( int state ) {}
    virtual void    StartSneaking( void ) {}
    virtual void    StopSneaking( void ) {}
    virtual BOOL	OnControls( entvars_t *pev ) { return FALSE; }
    virtual BOOL    IsSneaking( void ) { return FALSE; }
    virtual BOOL	IsAlive( void ) { return (pev->deadflag == DEAD_NO) && pev->health > 0; }
    virtual BOOL	IsBSPModel( void ) { return pev->solid == SOLID_BSP || pev->movetype == MOVETYPE_PUSHSTEP; }
    virtual BOOL	ReflectGauss( void ) { return ( IsBSPModel() && !pev->takedamage ); }
    virtual BOOL	HasTarget( string_t targetname ) { return FStrEq(STRING(targetname), STRING(pev->targetname) ); }
    virtual BOOL    IsInWorld( void );
    virtual	BOOL	IsPlayer( void ) { return FALSE; }
    virtual BOOL	IsNetClient( void ) { return FALSE; }
    virtual const char *TeamID( void ) { return ""; }


//	virtual void	SetActivator( CBaseEntity *pActivator ) {}
    virtual CBaseEntity *GetNextTarget( void );

    // fundamental callbacks
    void (CBaseEntity ::*m_pfnThink)(void);
    void (CBaseEntity ::*m_pfnTouch)( CBaseEntity *pOther );
    void (CBaseEntity ::*m_pfnUse)( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
    void (CBaseEntity ::*m_pfnBlocked)( CBaseEntity *pOther );

    virtual void Think( void ) { if (m_pfnThink) (this->*m_pfnThink)(); };
    virtual void Touch( CBaseEntity *pOther ) { if (m_pfnTouch) (this->*m_pfnTouch)( pOther ); };
    virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
    {
        if (m_pfnUse)
            (this->*m_pfnUse)( pActivator, pCaller, useType, value );
    }
    virtual void Blocked( CBaseEntity *pOther ) { if (m_pfnBlocked) (this->*m_pfnBlocked)( pOther ); };

    // allow engine to allocate instance data
    void *operator new( size_t stAllocateBlock, entvars_t *pev )
    {
        return (void *)ALLOC_PRIVATE(ENT(pev), stAllocateBlock);
    };

    // don't use this.
#if _MSC_VER >= 1200 // only build this code if MSVC++ 6.0 or higher
    void operator delete(void *pMem, entvars_t *pev)
    {
        pev->flags |= FL_KILLME;
    };
#endif

    void UpdateOnRemove( void );

    // common member functions
    void EXPORT SUB_Remove( void );
    void EXPORT SUB_DoNothing( void );
    void EXPORT SUB_StartFadeOut ( void );
    void EXPORT SUB_FadeOut ( void );
    void EXPORT SUB_CallUseToggle( void ) { this->Use( this, this, USE_TOGGLE, 0 ); }
    int			ShouldToggle( USE_TYPE useType, BOOL currentState );
    void		FireBullets( ULONG	cShots, Vector  vecSrc, Vector	vecDirShooting,	Vector	vecSpread, float flDistance,
int iBulletType, int iTracerFreq = 4, int iDamage = 0, entvars_t *pevAttacker = NULL  ); Vector		FireBulletsPlayer(
ULONG	cShots, Vector  vecSrc, Vector	vecDirShooting,	Vector	vecSpread, float flDistance, int iBulletType, int
iTracerFreq = 4, int iDamage = 0, entvars_t *pevAttacker = NULL, int shared_rand = 0 );

    virtual CBaseEntity *Respawn( void ) { return NULL; }

    void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );
    // Do the bounding boxes of these two intersect?
    int		Intersects( CBaseEntity *pOther );
    void	MakeDormant( void );
    int		IsDormant( void );
    BOOL    IsLockedByMaster( void ) { return FALSE; }

    static CBaseEntity *Instance( edict_t *pent )
    {
        if ( !pent )
            pent = ENT(0);
        CBaseEntity *pEnt = (CBaseEntity *)GET_PRIVATE(pent);
        return pEnt;
    }

    static CBaseEntity *Instance( entvars_t *pev ) { return Instance( ENT( pev ) ); }
    static CBaseEntity *Instance( int eoffset) { return Instance( ENT( eoffset) ); }

    CBaseMonster *GetMonsterPointer( entvars_t *pevMonster )
    {
        CBaseEntity *pEntity = Instance( pevMonster );
        if ( pEntity )
            return pEntity->MyMonsterPointer();
        return NULL;
    }
    CBaseMonster *GetMonsterPointer( edict_t *pentMonster )
    {
        CBaseEntity *pEntity = Instance( pentMonster );
        if ( pEntity )
            return pEntity->MyMonsterPointer();
        return NULL;
    }

    // virtual functions used by a few classes

    // used by monsters that are created by the MonsterMaker
    virtual	void UpdateOwner( void ) { return; };


    //
    static CBaseEntity *Create( char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner =
NULL );

    virtual BOOL FBecomeProne( void ) {return FALSE;};
    edict_t *edict() { return ENT( pev ); };
    EOFFSET eoffset( ) { return OFFSET( pev ); };
    int	  entindex( ) { return ENTINDEX( edict() ); };

    virtual Vector Center( ) { return (pev->absmax + pev->absmin) * 0.5; }; // center point of entity
    virtual Vector EyePosition( ) { return pev->origin + pev->view_ofs; };			// position of eyes
    virtual Vector EarPosition( ) { return pev->origin + pev->view_ofs; };			// position of ears
    virtual Vector BodyTarget( const Vector &posSrc ) { return Center( ); };		// position to shoot at

    virtual int Illumination( ) { return GETENTITYILLUM( ENT( pev ) ); };

    virtual	BOOL FVisible ( CBaseEntity *pEntity );
    virtual	BOOL FVisible ( const Vector &vecOrigin );

    //We use this variables to store each ammo count.
    int ammo_9mm;
    int ammo_357;
    int ammo_bolts;
    int ammo_buckshot;
    int ammo_rockets;
    int ammo_uranium;
    int ammo_hornets;
    int ammo_argrens;
    //Special stuff for grenades and satchels.
    float m_flStartThrow;
    float m_flReleaseThrow;
    int m_chargeReady;
    int m_fInAttack;

    enum EGON_FIRESTATE { FIRE_OFF, FIRE_CHARGE };
    int m_fireState;
};
*/

namespace SPMod
{
    namespace Engine
    {
        class IEdict;
        class IEntVars;
    } // namespace Engine

    class IBaseEntity : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        std::string_view getName() const override
        {
            return "IBaseEntity";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual ~IBaseEntity() = default;
        virtual Engine::IEdict *edict() const = 0;
        virtual void remove() = 0;
        virtual bool isAlive() const = 0;
        virtual std::string_view getTeam() const = 0;
        virtual int takeHealth(float flHealth, int bitsDamageType) const = 0;
        virtual int takeDamage(Engine::IEntVars *pevInflictor,
                               Engine::IEntVars *pevAttacker,
                               float flDamage,
                               int bitsDamageType) = 0;
    };
} // namespace SPMod