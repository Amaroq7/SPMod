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

/*class CBaseToggle : public CBaseAnimating
{
public:
    void KeyValue(KeyValueData *pkvd);

    TOGGLE_STATE m_toggle_state;
    float m_flActivateFinished; // like attack_finished, but for doors
    float m_flMoveDistance;     // how far a door should slide or rotate
    float m_flWait;
    float m_flLip;
    float m_flTWidth;  // for plats
    float m_flTLength; // for plats

    Vector m_vecPosition1;
    Vector m_vecPosition2;
    Vector m_vecAngle1;
    Vector m_vecAngle2;

    int m_cTriggersLeft; // trigger_counter only, # of activations remaining
    float m_flHeight;
    EHANDLE m_hActivator;
    void (CBaseToggle::*m_pfnCallWhenMoveDone)(void);
    Vector m_vecFinalDest;
    Vector m_vecFinalAngle;

    int m_bitsDamageInflict; // DMG_ damage type that the door or tigger does

    virtual int Save(CSave &save);
    virtual int Restore(CRestore &restore);

    static TYPEDESCRIPTION m_SaveData[];

    virtual int GetToggleState(void)
    {
        return m_toggle_state;
    }
    virtual float GetDelay(void)
    {
        return m_flWait;
    }

    // common member functions
    void LinearMove(Vector vecDest, float flSpeed);
    void EXPORT LinearMoveDone(void);
    void AngularMove(Vector vecDestAngle, float flSpeed);
    void EXPORT AngularMoveDone(void);
    BOOL IsLockedByMaster(void);

    static float AxisValue(int flags, const Vector &angles);
    static void AxisDir(entvars_t *pev);
    static float AxisDelta(int flags, const Vector &angle1, const Vector &angle2);

    string_t m_sMaster; // If this button has a master switch, this is the targetname.
                        // A master switch must be of the multisource type. If all
                        // of the switches in the multisource have been triggered, then
                        // the button will be allowed to operate. Otherwise, it will be
                        // deactivated.
};*/

namespace SPMod
{
    class IBaseToggle : public virtual IBaseAnimating
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
            return "IBaseToggle";
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

        virtual ~IBaseToggle() = default;
    };
} // namespace SPMod