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

class CBaseAnimating : public CBaseDelay
{
public:
    virtual int		Save( CSave &save );
    virtual int		Restore( CRestore &restore );

    static	TYPEDESCRIPTION m_SaveData[];

    // Basic Monster Animation functions
    float StudioFrameAdvance( float flInterval = 0.0 ); // accumulate animation frame time from last time called until
now int	 GetSequenceFlags( void ); int  LookupActivity ( int activity ); int  LookupActivityHeaviest ( int activity );
    int  LookupSequence ( const char *label );
    void ResetSequenceInfo ( );
    void DispatchAnimEvents ( float flFutureInterval = 0.1 ); // Handle events that have happend since last time called
up until X seconds into the future virtual void HandleAnimEvent( MonsterEvent_t *pEvent ) { return; }; float
SetBoneController ( int iController, float flValue ); void InitBoneControllers ( void ); float SetBlending ( int
iBlender, float flValue ); void GetBonePosition ( int iBone, Vector &origin, Vector &angles ); void GetAutomovement(
Vector &origin, Vector &angles, float flInterval = 0.1 ); int  FindTransition( int iEndingSequence, int iGoalSequence,
int *piDir ); void GetAttachment ( int iAttachment, Vector &origin, Vector &angles ); void SetBodygroup( int iGroup, int
iValue ); int GetBodygroup( int iGroup ); int ExtractBbox( int sequence, float *mins, float *maxs ); void
SetSequenceBox( void );

    // animation needs
    float				m_flFrameRate;		// computed FPS for current sequence
    float				m_flGroundSpeed;	// computed linear movement rate for current sequence
    float				m_flLastEventCheck;	// last time the event list was checked
    BOOL				m_fSequenceFinished;// flag set when StudioAdvanceFrame moves across a frame boundry
    BOOL				m_fSequenceLoops;	// true if the sequence loops
};
*/

namespace SPMod
{
    class IBaseAnimating : public virtual IBaseDelay
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
            return "IBaseAnimating";
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

        virtual ~IBaseAnimating() = default;
    };
} // namespace SPMod