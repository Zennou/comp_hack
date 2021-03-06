/**
 * @file server/channel/src/packets/game/Warp.cpp
 * @ingroup channel
 *
 * @author HACKfrost
 *
 * @brief Request from the client to warp to a selected warp point.
 *
 * This file is part of the Channel Server (channel).
 *
 * Copyright (C) 2012-2018 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Packets.h"

// libcomp Includes
#include <Log.h>
#include <ManagerPacket.h>
#include <Packet.h>
#include <PacketCodes.h>

// object Includes
#include <ActivatedAbility.h>
#include <Item.h>
#include <MiConditionData.h>
#include <MiSkillData.h>
#include <MiWarpPointData.h>

// channel Includes
#include "ChannelServer.h"

using namespace channel;

bool Parsers::Warp::Parse(libcomp::ManagerPacket *pPacketManager,
    const std::shared_ptr<libcomp::TcpConnection>& connection,
    libcomp::ReadOnlyPacket& p) const
{
    if(p.Size() != 9)
    {
        return false;
    }

    int32_t entityID = p.ReadS32Little();
    int8_t activationID = p.ReadS8();
    uint32_t warpPointID = p.ReadU32Little();

    auto client = std::dynamic_pointer_cast<ChannelClientConnection>(connection);
    auto state = client->GetClientState();
    auto sourceState = state->GetEntityState(entityID);

    if(sourceState == nullptr)
    {
        LOG_ERROR("Player attempted to warp an entity that does not belong"
            " to the client\n");
        state->SetLogoutSave(true);
        client->Close();
        return true;
    }

    auto server = std::dynamic_pointer_cast<ChannelServer>(pPacketManager->GetServer());
    auto definitionManager = server->GetDefinitionManager();
    auto skillManager = server->GetSkillManager();
    auto zoneManager = server->GetZoneManager();

    auto activatedAbility = sourceState->GetActivatedAbility();
    if(!activatedAbility || activatedAbility->GetActivationID() != activationID)
    {
        LOG_ERROR("Invalid activation ID encountered for Warp request\n");
    }
    else
    {
        auto item = std::dynamic_pointer_cast<objects::Item>(
            libcomp::PersistentObject::GetObjectByUUID(
            state->GetObjectUUID(activatedAbility->GetTargetObjectID())));

        auto warpDef = definitionManager->GetWarpPointData(warpPointID);
        auto skillData = definitionManager->GetSkillData(activatedAbility->GetSkillID());
        if(warpDef && item)
        {
            uint32_t zoneID = warpDef->GetZoneID();

            float x = warpDef->GetX();
            float y = warpDef->GetY();
            float rot = warpDef->GetRotation();

            skillManager->ExecuteSkill(sourceState, (uint8_t)activationID,
                activatedAbility->GetTargetObjectID());

            // Some of the warp items without expirations need to be consumed
            // but are not skill costs
            /// @todo: look into why this is and possibly correct binary data
            if(item->GetRentalExpiration() == 0 &&
                skillData->GetCondition()->CostsCount() == 0)
            {
                std::list<std::shared_ptr<objects::Item>> empty;

                std::unordered_map<std::shared_ptr<objects::Item>, uint16_t> updates;
                updates[item] = (uint16_t)(item->GetStackSize() - 1);

                server->GetCharacterManager()->UpdateItems(client, false, empty, updates);
            }

            zoneManager->EnterZone(client, zoneID, 0, x, y, rot);
        }
        else
        {
            skillManager->CancelSkill(sourceState, (uint8_t)activationID);
        }
    }

    return true;
}
