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
//
// generic Delay entity.
//
class CBaseDelay : public CBaseEntity
{
public:
    float		m_flDelay;
    int			m_iszKillTarget;

    virtual void	KeyValue( KeyValueData* pkvd);
    virtual int		Save( CSave &save );
    virtual int		Restore( CRestore &restore );

    static	TYPEDESCRIPTION m_SaveData[];
    // common member functions
    void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );
    void EXPORT DelayThink( void );
};

*/

namespace SPMod
{
    class IBaseDelay : public virtual IBaseEntity
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
            return "IBaseDelay";
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

        virtual ~IBaseDelay() = default;
    };
} // namespace SPMod