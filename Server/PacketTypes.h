#pragma once

enum PacketType :int16_t // NOTE: (reductor) This is used as an 'enum class' yet not defined as one, this might become an issue with different compilers
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