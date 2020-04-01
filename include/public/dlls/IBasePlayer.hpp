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
class CBasePlayer : public CBaseMonster
{
public:

    // Spectator camera
    void	Observer_FindNextPlayer( bool bReverse );
    void	Observer_HandleButtons();
    void	Observer_SetMode( int iMode );
    void	Observer_CheckTarget();
    void	Observer_CheckProperties();
    EHANDLE	m_hObserverTarget;
    float	m_flNextObserverInput;
    int		m_iObserverWeapon;	// weapon of current tracked target
    int		m_iObserverLastMode;// last used observer mode
    int		IsObserver() { return pev->iuser1; };
    int					random_seed;    // See that is shared between client & server for shared weapons code

    int					m_iPlayerSound;// the index of the sound list slot reserved for this player
    int					m_iTargetVolume;// ideal sound volume.
    int					m_iWeaponVolume;// how loud the player's weapon is right now.
    int					m_iExtraSoundTypes;// additional classification for this weapon's sound
    int					m_iWeaponFlash;// brightness of the weapon flash
    float				m_flStopExtraSoundTime;

    float				m_flFlashLightTime;	// Time until next battery draw/Recharge
    int					m_iFlashBattery;		// Flashlight Battery Draw

    int					m_afButtonLast;
    int					m_afButtonPressed;
    int					m_afButtonReleased;

    edict_t			   *m_pentSndLast;			// last sound entity to modify player room type
    float				m_flSndRoomtype;		// last roomtype set by sound entity
    float				m_flSndRange;			// dist from player to sound entity

    float				m_flFallVelocity;

    int					m_rgItems[MAX_ITEMS];
    int					m_fKnownItem;		// True when a new item needs to be added
    int					m_fNewAmmo;			// True when a new item has been added

    unsigned int		m_afPhysicsFlags;	// physics flags - set when 'normal' physics should be revisited or
overriden float				m_fNextSuicideTime; // the time after which the player can next use the suicide command


// these are time-sensitive things that we keep track of
    float				m_flTimeStepSound;	// when the last stepping sound was made
    float				m_flTimeWeaponIdle; // when to play another weapon idle animation.
    float				m_flSwimTime;		// how long player has been underwater
    float				m_flDuckTime;		// how long we've been ducking
    float				m_flWallJumpTime;	// how long until next walljump

    float				m_flSuitUpdate;					// when to play next suit update
    int					m_rgSuitPlayList[CSUITPLAYLIST];// next sentencenum to play for suit update
    int					m_iSuitPlayNext;				// next sentence slot for queue storage;
    int					m_rgiSuitNoRepeat[CSUITNOREPEAT];		// suit sentence no repeat list
    float				m_rgflSuitNoRepeatTime[CSUITNOREPEAT];	// how long to wait before allowing repeat
    int					m_lastDamageAmount;		// Last damage taken
    float				m_tbdPrev;				// Time-based damage timer

    float				m_flgeigerRange;		// range to nearest radiation source
    float				m_flgeigerDelay;		// delay per update of range msg to client
    int					m_igeigerRangePrev;
    int					m_iStepLeft;			// alternate left/right foot stepping sound
    char				m_szTextureName[CBTEXTURENAMEMAX];	// current texture name we're standing on
    char				m_chTextureType;		// current texture type

    int					m_idrowndmg;			// track drowning damage taken
    int					m_idrownrestored;		// track drowning damage restored

    int					m_bitsHUDDamage;		// Damage bits for the current fame. These get sent to
                                                // the hude via the DAMAGE message
    BOOL				m_fInitHUD;				// True when deferred HUD restart msg needs to be sent
    BOOL				m_fGameHUDInitialized;
    int					m_iTrain;				// Train control position
    BOOL				m_fWeapon;				// Set this to FALSE to force a reset of the current weapon HUD info

    EHANDLE				m_pTank;				// the tank which the player is currently controlling,  NULL if no tank
    float				m_fDeadTime;			// the time at which the player died  (used in PlayerDeathThink())

    BOOL			m_fNoPlayerSound;	// a debugging feature. Player makes no sound if this is true.
    BOOL			m_fLongJump; // does this player have the longjump module?

    float       m_tSneaking;
    int			m_iUpdateTime;		// stores the number of frame ticks before sending HUD update messages
    int			m_iClientHealth;	// the health currently known by the client.  If this changes, send a new
    int			m_iClientBattery;	// the Battery currently known by the client.  If this changes, send a new
    int			m_iHideHUD;		// the players hud weapon info is to be hidden
    int			m_iClientHideHUD;
    int			m_iFOV;			// field of view
    int			m_iClientFOV;	// client's known FOV
    // usable player items
    CBasePlayerItem	*m_rgpPlayerItems[MAX_ITEM_TYPES];
    CBasePlayerItem *m_pActiveItem;
    CBasePlayerItem *m_pClientActiveItem;  // client version of the active item
    CBasePlayerItem *m_pLastItem;
    // shared ammo slots
    int	m_rgAmmo[MAX_AMMO_SLOTS];
    int	m_rgAmmoLast[MAX_AMMO_SLOTS];

    Vector				m_vecAutoAim;
    BOOL				m_fOnTarget;
    int					m_iDeaths;
    float				m_iRespawnFrames;	// used in PlayerDeathThink() to make sure players can always respawn

    int m_lastx, m_lasty;  // These are the previous update's crosshair angles, DON"T SAVE/RESTORE

    int m_nCustomSprayFrames;// Custom clan logo frames for this player
    float	m_flNextDecalTime;// next time this player can spray a decal

    char m_szTeamName[TEAM_NAME_LENGTH];

    virtual void Spawn( void );
    void Pain( void );

//	virtual void Think( void );
    virtual void Jump( void );
    virtual void Duck( void );
    virtual void PreThink( void );
    virtual void PostThink( void );
    virtual Vector GetGunPosition( void );
    virtual int TakeHealth( float flHealth, int bitsDamageType );
    virtual void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
bitsDamageType); virtual int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int
bitsDamageType); virtual void	Killed( entvars_t *pevAttacker, int iGib ); virtual Vector BodyTarget( const Vector
&posSrc ) { return Center( ) + pev->view_ofs * RANDOM_FLOAT( 0.5, 1.1 ); };		// position to shoot at virtual void
StartSneaking( void ) { m_tSneaking = gpGlobals->time - 1; } virtual void StopSneaking( void ) { m_tSneaking =
gpGlobals->time + 30; } virtual BOOL IsSneaking( void ) { return m_tSneaking <= gpGlobals->time; } virtual BOOL
ShouldFadeOnDeath( void ) { return FALSE;
}
    virtual	BOOL IsPlayer( void ) { return TRUE; }			// Spectators should return FALSE for this, they aren't
"players" as far as game logic is concerned

    virtual BOOL IsNetClient( void ) { return TRUE; }		// Bots should return FALSE for this, they can't receive NET
messages
                                                            // Spectators should return TRUE for this
    virtual const char *TeamID( void );

    virtual int		Save( CSave &save );
    virtual int		Restore( CRestore &restore );
    void RenewItems(void);
    void PackDeadPlayerItems( void );
    void RemoveAllItems( BOOL removeSuit );
    BOOL SwitchWeapon( CBasePlayerItem *pWeapon );

    // JOHN:  sends custom messages if player HUD data has changed  (eg health, ammo)
    virtual void UpdateClientData( void );

    static	TYPEDESCRIPTION m_playerSaveData[];

    // Player is moved across the transition by other means
    virtual int		ObjectCaps( void ) { return CBaseMonster :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
    virtual void	Precache( void );
    BOOL			IsOnLadder( void );
    BOOL			FlashlightIsOn( void );
    void			FlashlightTurnOn( void );
    void			FlashlightTurnOff( void );

    void UpdatePlayerSound ( void );
    void DeathSound ( void );

    int Classify ( void );
    void SetAnimation( PLAYER_ANIM playerAnim );
    void SetWeaponAnimType( const char *szExtention );
    char m_szAnimExtention[32];

    // custom player functions
    virtual void ImpulseCommands( void );
    void CheatImpulseCommands( int iImpulse );

    void StartDeathCam( void );
    void StartObserver( Vector vecPosition, Vector vecViewAngle );

    void AddPoints( int score, BOOL bAllowNegativeScore );
    void AddPointsToTeam( int score, BOOL bAllowNegativeScore );
    BOOL AddPlayerItem( CBasePlayerItem *pItem );
    BOOL RemovePlayerItem( CBasePlayerItem *pItem );
    void DropPlayerItem ( char *pszItemName );
    BOOL HasPlayerItem( CBasePlayerItem *pCheckItem );
    BOOL HasNamedPlayerItem( const char *pszItemName );
    BOOL HasWeapons( void );// do I have ANY weapons?
    void SelectPrevItem( int iItem );
    void SelectNextItem( int iItem );
    void SelectLastItem(void);
    void SelectItem(const char *pstr);
    void ItemPreFrame( void );
    void ItemPostFrame( void );
    void GiveNamedItem( const char *szName );
    void EnableControl(BOOL fControl);

    int  GiveAmmo( int iAmount, char *szName, int iMax );
    void SendAmmoUpdate(void);

    void WaterMove( void );
    void EXPORT PlayerDeathThink( void );
    void PlayerUse( void );

    void CheckSuitUpdate();
    void SetSuitUpdate(char *name, int fgroup, int iNoRepeat);
    void UpdateGeigerCounter( void );
    void CheckTimeBasedDamage( void );

    BOOL FBecomeProne ( void );
    void BarnacleVictimBitten ( entvars_t *pevBarnacle );
    void BarnacleVictimReleased ( void );
    static int GetAmmoIndex(const char *psz);
    int AmmoInventory( int iAmmoIndex );
    int Illumination( void );

    void ResetAutoaim( void );
    Vector GetAutoaimVector( float flDelta  );
    Vector AutoaimDeflection( Vector &vecSrc, float flDist, float flDelta  );

    void ForceClientDllUpdate( void );  // Forces all client .dll specific data to be resent to client.

    void DeathMessage( entvars_t *pevKiller );

    void SetCustomDecalFrames( int nFrames );
    int GetCustomDecalFrames( void );

    void CBasePlayer::TabulateAmmo( void );

    float m_flStartCharge;
    float m_flAmmoStartCharge;
    float m_flPlayAftershock;
    float m_flNextAmmoBurn;// while charging, when to absorb another unit of player's ammo?

    //Player ID
    void InitStatusBar( void );
    void UpdateStatusBar( void );
    int m_izSBarState[ SBAR_END ];
    float m_flNextSBarUpdateTime;
    float m_flStatusBarDisappearDelay;
    char m_SbarString0[ SBAR_STRING_SIZE ];
    char m_SbarString1[ SBAR_STRING_SIZE ];

    float m_flNextChatTime;

};

*/

namespace SPMod
{
    class IBasePlayer : public virtual IBaseMonster
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
            return "IBasePlayer";
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

        virtual ~IBasePlayer() = default;
    };
} // namespace SPMod