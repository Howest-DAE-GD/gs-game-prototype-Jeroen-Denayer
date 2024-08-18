#include "pch.h"
#include "CatchMechanism.h"
#include "SingleClick.h"

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
