﻿#ifndef CSTEAMACH
#define CSTEAMACH
#define _ACH_ID(id,name) {id,#id,name,"",0,0}
#include <../steamworks/public/steam/steam_api.h>

struct Achievement_t {
	int         m_eAchievementID;
	const char* m_pchAchievementID;
	char        m_rgchName[128];
	char        m_rgchDescription[256];
	bool        m_bAchieved;
	int         m_iIconImage;
};

class CSteamAchievements {
private:
	int64          m_iAppID;
	Achievement_t* m_pAchievements;
	int            m_iNumAchievements;
	bool           m_bInitialized;

public:
	CSteamAchievements(Achievement_t* Achievements, int numAchievements);
	~CSteamAchievements(){}
	bool RequestStats();
	bool SetAchievement(const char* ID);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
	
};
#endif