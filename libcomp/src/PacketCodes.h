/**
 * @file libcomp/src/PacketCodes.h
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief Contains enums for internal and external packet codes.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2016 COMP_hack Team <compomega@tutanota.com>
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

#ifndef LIBCOMP_SRC_PACKETCODES_H
#define LIBCOMP_SRC_PACKETCODES_H

// Standard C++11 Includes
#include <stdint.h>
#include <type_traits>

/**
 * Request or response packet code between the server and game client.
 */
enum class LobbyClientPacketCode_t : uint16_t
{
    PACKET_LOGIN = 0x0003,  //!< Login request from the client.
    PACKET_LOGIN_RESPONSE = 0x0004, //!< Login response to the client.
    PACKET_AUTH = 0x0005,   //!< Authorization request from the client.
    PACKET_AUTH_RESPONSE = 0x0006,  //!< Authorization response to the client.
    PACKET_START_GAME = 0x0007, //!< Start game request from the client.
    PACKET_START_GAME_RESPONSE = 0x0008,    //!< Start game response to the client.
    PACKET_CHARACTER_LIST = 0x0009, //!< Character list request from the client.
    PACKET_CHARACTER_LIST_RESPONSE = 0x000A,    //!< Character list response to the client.
    PACKET_WORLD_LIST = 0x000B, //!< World list request from the client.
    PACKET_WORLD_LIST_RESPONSE = 0x000C,    //!< World list response to the client.
    PACKET_CREATE_CHARACTER = 0x000D,   //!< Create character request from the client.
    PACKET_CREATE_CHARACTER_RESPONSE = 0x000E,  //!< Create character response to the client.
    PACKET_DELETE_CHARACTER = 0x000F,   //!< Delete character request from the client.
    PACKET_DELETE_CHARACTER_RESPONSE = 0x0010,  //!< Delete character response to the client.
    PACKET_QUERY_PURCHASE_TICKET = 0x0011,  //!< Query purchase ticket request from the client.
    PACKET_QUERY_PURCHASE_TICKET_RESPONSE = 0x0012, //!< Query purchase ticket response to the client.
    PACKET_PURCHASE_TICKET = 0x0013,    //!< Purchase ticket request from the client.
    PACKET_PURCHASE_TICKET_RESPONSE = 0x0014,   //!< Purchase ticket response to the client.
};

/**
 * Request or response packet code between two internal servers.
 */
enum class InternalPacketCode_t : uint16_t
{
    PACKET_DESCRIBE_WORLD = 0x1001, //!< Request to describe the world server.
    PACKET_SET_WORLD_DESCRIPTION = 0x1002,  //!< Request to update a non-world server's world information.
    PACKET_SET_CHANNEL_DESCRIPTION = 0x1003,    //!< Request to update a non-channel server's channel information.
};

/**
 * Mode signifier that changes how a packet should be handled.
 */
enum class InternalPacketAction_t : uint8_t
{
    PACKET_ACTION_ADD = 1,  //!< Packet action is an addition.
    PACKET_ACTION_UPDATE,   //!< Packet action is an update.
    PACKET_ACTION_REMOVE,   //!< Pacekt action is a removal.
};

/**
 * Casting utility to maintain an enum type when passed as a parameter.
 */
template<typename T>
constexpr typename std::underlying_type<T>::type to_underlying(T val)
{
    return static_cast<typename std::underlying_type<T>::type>(val);
}

#endif // LIBCOMP_SRC_PACKETCODES_H
