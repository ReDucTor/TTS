#pragma once

enum PacketType :int16_t
{
	Chat_ChatMessage,
	Login_AttemptToLogin,
	Login_WrongPassword,
	Login_NoSuchUser,
	Login_LoginSuccessful,
	Login_RequestUserData,
	Login_SendUserData,
	General_RequestGameInfo,
	Mysql_Error
};