/*
 * Copyright (C) 2010-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2010-2012 Oregon <http://www.oregoncore.com/>
 * Copyright (C) 2006-2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Lord_Alexei_Barov
SD%Complete: 100
SDComment: aura applied/defined in database
SDCategory: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

#define SPELL_IMMOLATE             20294                    // Old ID  was 15570
#define SPELL_VEILOFSHADOW         17820

struct boss_lordalexeibarovAI : public ScriptedAI
{
    boss_lordalexeibarovAI(Creature *c) : ScriptedAI(c) {}

    uint32 Immolate_Timer;
    uint32 VeilofShadow_Timer;

    void Reset()
    {
        Immolate_Timer = 7000;
        VeilofShadow_Timer = 15000;

        me->LoadCreaturesAddon();
    }

    void JustDied(Unit * /*killer*/)
    {
        ScriptedInstance *instance = me->GetInstanceScript();
        if (instance)
        {
            instance->SetData(DATA_LORDALEXEIBAROV_DEATH, 0);

            if (instance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void EnterCombat(Unit * /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Immolate_Timer
        if (Immolate_Timer <= diff)
        {
            Unit *pTarget = NULL;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if (pTarget) DoCast(pTarget, SPELL_IMMOLATE);

            Immolate_Timer = 12000;
        } else Immolate_Timer -= diff;

        //VeilofShadow_Timer
        if (VeilofShadow_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_VEILOFSHADOW);
            VeilofShadow_Timer = 20000;
        } else VeilofShadow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_lordalexeibarov(Creature* creature)
{
    return new boss_lordalexeibarovAI (creature);
}

void AddSC_boss_lordalexeibarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_alexei_barov";
    newscript->GetAI = &GetAI_boss_lordalexeibarov;
    newscript->RegisterSelf();
}

