#pragma once

class CAimbot
{
public:
	CAimbot()
	{
		iTargetBone = 1;
		iTargetBone2 = 1;
		iTargetBone3 = 1;
		iTargetPlayer = -1;
		iTargetPlayer2 = -1;
		iTargetPlayer3 = -1;
		bCrosshair = false;
		vecTargetBone = { 0.f, 0.f, 0.f };
		vecTargetBone2 = { 0.f, 0.f, 0.f };
		vecTargetBone3 = { 0.f, 0.f, 0.f };
		vecCrosshair = { 0.f, 0.f, 0.f };

		oFireInstantHit = (tFireInstantHit)(pSAMP->g_dwSAMP_Addr + 0xB05A0);
		oAddBullet = (tAddBullet)(pSAMP->g_dwSAMP_Addr + 0xA0BB0);
		oTargetWeaponRangeMultiplier = (tTargetWeaponRangeMultiplier)0x73B380;
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		pSecure->SDetourAttach(&(PVOID&)oFireInstantHit, hkFireInstantHit);
		pSecure->SDetourAttach(&(PVOID&)oAddBullet, hkAddBullet);
		DetourTransactionCommit();
		pSecure->HookInstallCall((DWORD)0x73FF11, (DWORD)hkTargetWeaponRangeMultiplier);
	};

	~CAimbot()
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)oFireInstantHit, hkFireInstantHit);
		DetourDetach(&(PVOID&)oAddBullet, hkAddBullet);
		DetourTransactionCommit();
		Memory::memcpy_safe((void*)0x73FF11, "\xE8\x6A\xB4\xFF\xFF", 5);
	};

	void Update();
	void Render();
	float CrossX = *(float*)0xB6EC14, CrossY = *(float*)0xB6EC10;

	CVector vecCrosshair;

private:
	bool bCrosshair;
	int iTargetPlayer, iTargetPlayer2, iTargetPlayer3, iTargetBone, iTargetBone2, iTargetBone3;
	CVector vecTargetBone, vecTargetBone2, vecTargetBone3;
	void GetAimingPlayer();
	void GetAimingPlayer2();
	void GetAimingPlayer3();
	void SmoothAimbot();
	void FlickShot();
	void Triggerbot();
	void Coords();

	typedef bool(__stdcall* tFireInstantHit)(void*, CEntity*, CVector*, CVector*, CEntity*, CVector*, CVector*, bool, bool);
	typedef bool(__cdecl* tAddBullet)(CEntity*, eWeaponType, CVector, CVector);
	typedef float(__cdecl* tTargetWeaponRangeMultiplier)(CEntity*, CEntity*);

	static bool __stdcall hkFireInstantHit(void* this_, CEntity* pFiringEntity, CVector* pOrigin, CVector* pMuzzle, CEntity* pTargetEntity, CVector* pTarget, CVector* pVec, bool bCrossHairGun, bool bCreateGunFx);
	static bool __cdecl hkAddBullet(CEntity* pCreator, eWeaponType weaponType, CVector vecPosition, CVector vecVelocity);
	static float __cdecl hkTargetWeaponRangeMultiplier(CEntity* pVictim, CEntity* pOwner);

	tFireInstantHit oFireInstantHit;
	tAddBullet oAddBullet;
	tTargetWeaponRangeMultiplier oTargetWeaponRangeMultiplier;
};

extern CAimbot* pAimbot;