#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 1.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GD)
{
	switch (_GD->m_GS)
	{
	case GS_PLAY_FPS_CAM:
	{
		pHeight = 15;
		if (m_pos.y - pHeight <= 0)
		{	
			m_pos.y = pHeight;
		}
		
		//TURN AND FORWARD CONTROL HERE
		Vector3 forwardMove = 40.0f * Vector3::Forward;
		Matrix rotMove = Matrix::CreateRotationY(m_yaw);
		forwardMove = Vector3::Transform(forwardMove, rotMove);
		if (_GD->m_KBS.W)
		{
			m_acc += forwardMove;
		}
		if (_GD->m_KBS.S)
		{
			m_acc -= forwardMove;
		}
		break;

		//{
		//	//MOUSE CONTROL SCHEME HERE
		//	float speed = 10.0f;
		//	m_acc.x += speed * _GD->m_MS.x;
		//	m_acc.z += speed * _GD->m_MS.y;
		//	break;
		//}
	}
	case GS_PLAY_TPS_CAM:
	{
		//TURN AND FORWARD CONTROL HERE
		Vector3 forwardMove = 40.0f * Vector3::Forward;
		Matrix rotMove = Matrix::CreateRotationY(m_yaw);
		forwardMove = Vector3::Transform(forwardMove, rotMove);
		if (_GD->m_KBS.W)
		{
			m_acc += forwardMove;
		}
		if (_GD->m_KBS.S)
		{
			m_acc -= forwardMove;
		}
		break;
	}
	}

	//change orinetation of player
	float rotSpeed = 2.0f * _GD->m_dt;
	Vector3 sideMove = 40.0f * Vector3::Right;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	sideMove = Vector3::Transform(sideMove, rotMove);
	m_yaw -= _GD->m_dt * _GD->m_MS.x;
	m_pitch -= _GD->m_dt * _GD->m_MS.y;
	if (m_pitch > XMConvertToRadians(60)) m_pitch = XMConvertToRadians(60);
	if (m_pitch < XMConvertToRadians(-60)) m_pitch = XMConvertToRadians(-60);
	if (_GD->m_KBS.A)
	{
		m_acc -= sideMove;
	}
	if (_GD->m_KBS.D)
	{
		m_acc += sideMove;
	}

	//move player up and down
	if (_GD->m_KBS.R)
	{
		m_acc.y += 40.0f;
	}

	if (_GD->m_KBS.F)
	{
		m_acc.y -= 40.0f;
	}

	//limit motion of the player
	float length = m_pos.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		m_pos.Normalize();
		m_pos *= maxLength;
		m_vel *= -0.9; //VERY simple bounce back
	}

	//apply my base behaviour
	CMOGO::Tick(_GD);
}