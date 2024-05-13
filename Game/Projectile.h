#pragma once
#include "CMOGO.h"

class Projectile : public CMOGO
{

public:
	Projectile(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);	

	~Projectile();

	virtual void Tick(GameData* _GD) override;

	//Matrix rotMove = Matrix::CreateRotationY(m_yaw);

protected:

	float lifetime = 0.0f;
	float maxLifetime = 5.0f;
};

