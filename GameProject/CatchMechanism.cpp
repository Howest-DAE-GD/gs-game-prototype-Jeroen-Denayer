#include "pch.h"
#include "CatchMechanism.h"
#include "SingleClick.h"

CatchMechanism* CatchMechanism::Create(Type type, Ball* pBall)
{
    return new SingleClick(pBall);
}

CatchMechanism::CatchMechanism(Type type, Ball* pBall)
    : m_Type{ type }
    , m_State{ State::Active }
    , m_pBall{ pBall }
{
}

CatchMechanism::~CatchMechanism()
{
}

CatchMechanism::State CatchMechanism::GetState() const
{
    return m_State;
}
