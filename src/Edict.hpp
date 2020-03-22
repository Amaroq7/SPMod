/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include "spmod.hpp"

class Edict : public virtual IEdict
{
public:
    Edict() = delete;
    Edict(edict_t *edict);
    Edict(std::uint32_t index);
    ~Edict() = default;

    // IEdict
    std::uint32_t getIndex() const override;
    std::string_view getClassName() const override;
    void getOrigin(float *origin) const override;
    void getVelocity(float *velocity) const override;
    void setVelocity(const float *velocity) override;
    float getHealth() const override;
    void setHealth(float health) override;
    void *getPrivateData() const override;

    // Edict
    edict_t *getInternalEdict() const;

protected:
    edict_t *m_edict;
};
